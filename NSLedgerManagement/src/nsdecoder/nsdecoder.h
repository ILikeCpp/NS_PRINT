#ifndef NSDECODER_H
#define NSDECODER_H

#include <QObject>
#include "nsdecoder_global.h"

class NSDecoderPrivate;
class NSDECODERSHARED_EXPORT NSDecoder: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool vibrateWhenTagFinded READ vibrateWhenTagFinded WRITE setVibrateWhenTagFinded NOTIFY vibrateWhenTagFindedChanged)
public:
    explicit NSDecoder(QObject *parent = 0);
    ~NSDecoder();

    static void registerQMLType();

    //Q_INVOKABLE void setCameraObject(QObject *cameraObj);

    /**
     *
     * Must start the decode thread by call startDecode() function before decode image.
     *
     */

    // start the decode thread
    Q_INVOKABLE void startDecode();
    // stop the decode thread
    Q_INVOKABLE void stopDecode();
    // check if the decode thread is running
    Q_INVOKABLE bool isRunning() const;

    // clip the img with maxWidth and maxHeight, then decode the cliped image,
    // if maxWidth = -1 or maxHeight = -1, img will not be cliped.
    Q_INVOKABLE bool decodeImage(const QImage &img, int maxWidth = -1, int maxHeight = -1);

    // clip the img with x,y,w,h, then decode the cliped image.
    Q_INVOKABLE bool decodeImage(const QImage &img, qreal x, qreal y, qreal w, qreal h);

    // clip the item with x,y,w,h, then decode the cliped item.
    // the item must be a visible QML Item.
    Q_INVOKABLE bool decodeImage(QObject *item, qreal x, qreal y, qreal w, qreal h);

    // vibrate if tag finded, the default value is true.
    bool vibrateWhenTagFinded() const;
    void setVibrateWhenTagFinded(bool vibrate);

signals:
    void started();

    // success is true means text not empty, else success is false.
    void decodeResult(bool success, const QString &text);

    void vibrateWhenTagFindedChanged();

private:
    NSDecoderPrivate * const d_ptr;
};

#endif // NSDECODER_H
