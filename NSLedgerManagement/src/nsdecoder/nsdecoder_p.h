#ifndef NSDECODERPRIVATE_H
#define NSDECODERPRIVATE_H

#include <QThread>
#include <QObject>
#include <QEvent>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include "QZXing.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

class DecodeWorker;
class NSDecoder;
class NSDecoderPrivate: public QThread
{
    Q_OBJECT
public:
    explicit NSDecoderPrivate(NSDecoder *q, QObject *parent = 0);
    ~NSDecoderPrivate();

    bool vibrateWhenTagFinded() const;
    void setVibrateWhenTagFinded(bool vibrate);

public slots:
    void onGrabReady();

protected:
    void run();

public:
    NSDecoder * const q_ptr;
    DecodeWorker *m_worker;
    bool m_vibrate;
    QSharedPointer<QQuickItemGrabResult> m_grabResult;
    qreal m_grabX;
    qreal m_grabY;
    qreal m_grabW;
    qreal m_grabH;
};

class DecodeWorker: public QObject
{
    Q_OBJECT
public:
    explicit DecodeWorker(QObject *parent = NULL);
    ~DecodeWorker();

    bool event(QEvent *evt);

    QString decodeByZXingRGB(const QImage &img);
    QString decodeByZXingYUV(const QImage &img);

    void setVibrate(bool vibrate);
    bool isVibrate();

signals:
    void decodeFinished(bool ok, const QString &text);

private slots:
    void onError(const QString &error);

private:
    void processImage(const QImage &img, int maxW = -1, int maxH = -1);

private:
#ifdef Q_OS_ANDROID
    QAndroidJniObject m_zxingDecoder;
#else
    QZXing m_qzxingDecoder;
#endif

    bool m_vibrate;
};

class DecodeImageEvent: public QEvent
{
public:
    DecodeImageEvent();

    void setDecodeImage(const QImage &img, int maxW, int maxH);
    QImage image() const;
    int maxWidth() const;
    int maxHeight() const;

private:
    QImage m_img;
    int m_width;
    int m_height;
};

class VibrateEvent: public QEvent
{
public:
    VibrateEvent();

    bool vibrate() const;
    void setVibrate(bool v);

private:
    bool m_vibrate;
};

#endif // NSDECODERPRIVATE_H
