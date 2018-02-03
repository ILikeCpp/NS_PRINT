#include "nsdecoder.h"
#include "nsglobal.h"
#include "nsdecoder_p.h"

#include <QCamera>
#include <QVideoProbe>
#include <QCameraImageCapture>
#include <QCameraViewfinderSettings>
#include <QVideoSurfaceFormat>
#include <QMediaObject>

#include "nsvideofilter.h"

NSDecoder::NSDecoder(QObject *parent):
    QObject(parent),
    d_ptr( new NSDecoderPrivate(this) )
{
    connect(d_ptr, SIGNAL(started()), this, SIGNAL(started()));
}

NSDecoder::~NSDecoder()
{
    if(d_ptr->isRunning())
    {
        d_ptr->quit();
        d_ptr->wait(100);
    }
    delete d_ptr;
}

void NSDecoder::registerQMLType()
{
    qmlRegisterType<NSDecoder>(NS_QML_PACKAGE, NS_QML_MAJOR_VER, NS_QML_MINOR_VER, "NSDecoder");
    qmlRegisterType<NSVideoFilter>(NS_QML_PACKAGE, NS_QML_MAJOR_VER, NS_QML_MINOR_VER, "NSVideoFilter");
}
/*
void NSDecoder::setCameraObject(QObject *cameraObj)
{
    qDebug() << "NSDecoder::setCameraObject";
    QCamera *camera = qvariant_cast<QCamera *>(cameraObj->property("mediaObject"));
    //camera->unload();

    QCameraViewfinderSettings setting = camera->viewfinderSettings();
    qDebug() << "resolution:" << setting.resolution();
    qDebug() << "minimumFrameRate:" << setting.minimumFrameRate();
    qDebug() << "maximumFrameRate:" << setting.maximumFrameRate();
    //setting.setResolution(1280,720);
    camera->setViewfinderSettings(setting);

    QList<QSize> res = camera->supportedViewfinderResolutions();
    qDebug() << "supportedViewfinderResolutions: " << res.size();
    for(int i = 0; i < res.size(); ++i)
    {
        qDebug() << res.at(i);
    }
    QList<QCameraViewfinderSettings> settings = camera->supportedViewfinderSettings();
    qDebug() << "supportedViewfinderSettings:" << settings.size();
    for(int i = 0; i < settings.size(); ++i)
    {
        qDebug() << settings.at(i).resolution();
    }
    //camera->start();
}
*/
void NSDecoder::startDecode()
{
    d_ptr->start();
}

void NSDecoder::stopDecode()
{
    d_ptr->quit();
}

bool NSDecoder::isRunning() const
{
    return d_ptr->isRunning();
}

bool NSDecoder::decodeImage(const QImage &img, int maxWidth, int maxHeight)
{
    if(NULL == d_ptr->m_worker)
    {
        emit decodeResult(false, QString());
        return false;
    }

    DecodeImageEvent *e = new DecodeImageEvent;
    e->setDecodeImage(img, maxWidth, maxHeight);
    QCoreApplication::postEvent(d_ptr->m_worker, e);

    return true;
}

//#include <QPainter>
bool NSDecoder::decodeImage(const QImage &img, qreal x, qreal y, qreal w, qreal h)
{
//    QImage f = img.copy();
//    QPainter painter(&f);
//    painter.setPen(Qt::red);
//    painter.drawRect(x,y,w,h);
//    QString name = ns_getAppDataPath() + "/" + QTime::currentTime().toString("HHmmsszzz") + ".jpg";
//    f.save(name);
    QImage clipImg = img.copy(x,y,w,h);
    return decodeImage(clipImg, w, h);
}

bool NSDecoder::decodeImage(QObject *item, qreal x, qreal y, qreal w, qreal h)
{
    if(NULL == d_ptr->m_worker)
    {
        emit decodeResult(false, QString());
        return false;
    }

    QQuickItem *pItem = qobject_cast<QQuickItem *>(item);
    if(NULL == pItem)
    {
        emit decodeResult(false, QString());
        return false;
    }

    d_ptr->m_grabResult = pItem->grabToImage();
    d_ptr->m_grabX = x;
    d_ptr->m_grabY = y;
    d_ptr->m_grabW = w;
    d_ptr->m_grabH = h;
    QQuickItemGrabResult *result = d_ptr->m_grabResult.data();
    if(NULL != result)
    {
        connect(result, SIGNAL(ready()), d_ptr, SLOT(onGrabReady()));
        return true;
    }
    else
    {
        qDebug() << "NSDecoder::decodeImage(): grab item to image failure";
        emit decodeResult(false, QString());
        return false;
    }
}

bool NSDecoder::vibrateWhenTagFinded() const
{
    return d_ptr->vibrateWhenTagFinded();
}

void NSDecoder::setVibrateWhenTagFinded(bool vibrate)
{
    d_ptr->setVibrateWhenTagFinded(vibrate);
}
