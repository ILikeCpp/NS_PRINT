#include "nsvideofilter.h"
#include "nsglobal.h"
#include <QDebug>
#include <QTime>
#include <QReadLocker>
#include <QWriteLocker>

#include <QImage>
#include <QAbstractVideoBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

/*
  Returns a QImage that wraps the given video frame.

  This is suitable only for QAbstractVideoBuffer::NoHandle frames with RGB (or BGR)
  data. YUV is not supported here.

  The QVideoFrame must be mapped and kept mapped as long as the wrapping QImage
  exists.

  As a convenience the function also supports frames with a handle type of
  QAbstractVideoBuffer::GLTextureHandle. This allows creating a system memory backed
  QVideoFrame containing the image data from an OpenGL texture. However, readback is a
  slow operation and may stall the GPU pipeline and should be avoided in production code.
*/
QImage imageWrapper(const QVideoFrame &frame)
{
#ifndef QT_NO_OPENGL
    if (frame.handleType() == QAbstractVideoBuffer::GLTextureHandle) {
        // Slow and inefficient path. Ideally what's on the GPU should remain on the GPU, instead of readbacks like this.
        QImage img(frame.width(), frame.height(), QImage::Format_RGBA8888);
        GLuint textureId = frame.handle().toUInt();
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, frame.width(), frame.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        return img;
    } else
#endif // QT_NO_OPENGL
    {
        if (!frame.isReadable()) {
            qWarning("imageFromVideoFrame: No mapped image data available for read");
            return QImage();
        }

        QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
        if (fmt != QImage::Format_Invalid)
            return QImage(frame.bits(), frame.width(), frame.height(), fmt);

        qWarning("imageFromVideoFrame: No matching QImage format");
    }

    return QImage();
}

NSVideoFilterRunnable::NSVideoFilterRunnable(NSVideoFilter *filter): m_filter(filter)
{
}

QVideoFrame NSVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    (void)surfaceFormat;(void)flags;
    //qDebug() << "input:" << input->size() << input->handleType() << surfaceFormat /*<< flags*/;

    if( (NULL != m_filter) && m_filter->needGrab() )
    {
        if(input->isValid())
        {
            input->map(QAbstractVideoBuffer::ReadOnly);

            QImage img = imageWrapper(*input);
            //qDebug() << img.format();
            //QString fileName = ns_getAppDataPath() + "/" + QTime::currentTime().toString("HHmmsszzz") + ".jpg";
            //qDebug() << fileName << img.save(fileName);
            m_filter->setNeedGrab(false);
            m_filter->imageReady(img.copy());

            input->unmap();
        }
    }

    return *input;
}


NSVideoFilter::NSVideoFilter(QObject *parent): QAbstractVideoFilter(parent)
{
    m_needGrab = false;
}

QVideoFilterRunnable *NSVideoFilter::createFilterRunnable()
{
    return new NSVideoFilterRunnable(this);
}

bool NSVideoFilter::needGrab()
{
    QReadLocker locker(&m_readwriteLock);
    return m_needGrab;
}

void NSVideoFilter::setNeedGrab(bool need)
{
    QWriteLocker locker(&m_readwriteLock);
    if(m_needGrab != need)
    {
        m_needGrab = need;
    }
}
