#ifndef NSVIDEOFILTER_H
#define NSVIDEOFILTER_H

#include "nsdecoder_global.h"
#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include <QReadWriteLock>

class NSVideoFilter;
class NSDECODERSHARED_EXPORT NSVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    NSVideoFilterRunnable(NSVideoFilter *filter);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    NSVideoFilter *m_filter;
};

class NSDECODERSHARED_EXPORT NSVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    NSVideoFilter(QObject *parent = 0);

    QVideoFilterRunnable *createFilterRunnable();

    // Check if need grab image, when NSVideoFilterRunnable grab the image, this will be set false.
    Q_INVOKABLE bool needGrab();
    // Set need grab flag
    Q_INVOKABLE void setNeedGrab(bool need);

signals:
    // emit when NSVideoFilterRunnable grab the image and then need grab flag set be false.
    void imageReady(const QImage &image);

private:
    bool m_needGrab;
    QReadWriteLock m_readwriteLock;
};

#endif // NSVIDEOFILTER_H
