#include "nsdecoder_p.h"
#include "nsdecoder.h"

static const QEvent::Type g_eventType_decodeImage = static_cast<QEvent::Type>(QEvent::registerEventType());
static const QEvent::Type g_eventType_vibrate = static_cast<QEvent::Type>(QEvent::registerEventType());

// Check JNI exception and clear it:
#define JNIENV_CHECK_EXCEPTION() \
do{ \
    QAndroidJniEnvironment env; \
    if(env->ExceptionCheck()) \
    { \
        env->ExceptionClear(); \
    } \
}while(0)

//让手机振动
inline void ns_vibrate(int ms)
{
    (void)ms;
#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField( "android/content/Context", "VIBRATOR_SERVICE", "Ljava/lang/String;");
    JNIENV_CHECK_EXCEPTION();
    QAndroidJniObject vibrateService = activity.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", name.object<jstring>());
    JNIENV_CHECK_EXCEPTION();
    jlong duration = ms;
    vibrateService.callMethod<void>("vibrate", "(J)V", duration);
    JNIENV_CHECK_EXCEPTION();
#endif
}

NSDecoderPrivate::NSDecoderPrivate(NSDecoder *q, QObject *parent):
    QThread(parent),
    q_ptr(q),
    m_worker(NULL),
    m_vibrate(true)
{
}

NSDecoderPrivate::~NSDecoderPrivate()
{
    if(isRunning())
    {
        quit();
        wait(100);
    }
}

bool NSDecoderPrivate::vibrateWhenTagFinded() const
{
    return m_vibrate;
}

void NSDecoderPrivate::setVibrateWhenTagFinded(bool vibrate)
{
    if(m_vibrate != vibrate)
    {
        m_vibrate = vibrate;
        if(NULL != m_worker)
        {
            VibrateEvent *e = new VibrateEvent;
            e->setVibrate(m_vibrate);
            QCoreApplication::postEvent(m_worker, e);
        }
        q_ptr->vibrateWhenTagFindedChanged();
    }
}

void NSDecoderPrivate::onGrabReady()
{
    if(NULL == m_worker)
    {
        emit q_ptr->decodeResult(false, QString());
        return;
    }

    QImage img = m_grabResult->image().copy(m_grabX, m_grabY, m_grabW, m_grabH);
    DecodeImageEvent *e = new DecodeImageEvent;
    e->setDecodeImage(img, m_grabW, m_grabH);
    QCoreApplication::postEvent(m_worker, e);

    m_grabX = 0.0;
    m_grabY = 0.0;
    m_grabW = 0.0;
    m_grabH = 0.0;
    m_grabResult.clear();
}

void NSDecoderPrivate::run()
{
    if(NULL != m_worker)
    {
        delete m_worker;
        m_worker = NULL;
    }
    m_worker = new DecodeWorker;
    m_worker->setVibrate(m_vibrate);
    connect(m_worker, SIGNAL(decodeFinished(bool,QString)), q_ptr, SIGNAL(decodeResult(bool,QString)));
    exec();

    delete m_worker;
    m_worker = NULL;
}

DecodeWorker::DecodeWorker(QObject *parent): QObject(parent)
{
#ifdef Q_OS_ANDROID
    m_zxingDecoder = QAndroidJniObject("NSCom/ZXingDecoder");
#else
    m_qzxingDecoder.setDecoder(QZXing::DecoderFormat_QR_CODE);
    connect(&m_qzxingDecoder, SIGNAL(error(QString)), this, SLOT(onError(QString)));
#endif
}

DecodeWorker::~DecodeWorker()
{
}

bool DecodeWorker::event(QEvent *evt)
{
    if(g_eventType_decodeImage == evt->type())
    {
        evt->setAccepted(true);
        DecodeImageEvent *e = static_cast<DecodeImageEvent *>(evt);
        if(NULL != e)
        {
            processImage(e->image().convertToFormat(QImage::Format_RGBA8888), e->maxWidth(), e->maxHeight());
        }
        return true;
    }
    if(g_eventType_vibrate == evt->type())
    {
        evt->setAccepted(true);
        VibrateEvent *e = static_cast<VibrateEvent *>(evt);
        if(NULL != e)
        {
            setVibrate(e->vibrate());
        }
        return true;
    }

    return QObject::event(evt);
}

QString DecodeWorker::decodeByZXingRGB(const QImage &img)
{
    if(img.size().isEmpty())
        return QString();

    QString str;

#ifdef Q_OS_ANDROID
    if(!m_zxingDecoder.isValid())
    {
        qDebug() << "DecodeWorker::decodeByZXingRGB(): decoder invalid";
        return QString();
    }

    const uchar *cImg =  img.constBits();
    const int width = img.width();
    const int height = img.height();
    const int size = width * height;
    jint *newdata = new jint[size];
    for(int i = 0; i < size; i ++)
    {
        /**
        在 Java 中获得 ARGB 像素的方法如下：
        public static int getARGB( int r, int g, int b, int alpha) {
            return (alpha << 24) | (r << 16) | (g << 8) | b;
        }

        img format is: QImage::Format_RGBA8888
        a: cImg[i*4 + 3]
        r: cImg[i*4 + 0]
        g: cImg[i*4 + 1]
        b: cImg[i*4 + 2]
        **/

        newdata[i] = (cImg[i*4 + 3] << 24) | (cImg[i*4 + 0] << 16) | (cImg[i*4 + 1] << 8 ) | (cImg[i*4 + 2]);
    }

    QAndroidJniEnvironment env;
    jintArray iarray = env->NewIntArray(size);
    env->SetIntArrayRegion(iarray, 0, size, newdata);

    QAndroidJniObject result = m_zxingDecoder.callObjectMethod("decodeRGBData", "([III)Ljava/lang/String;", iarray, width, height);
    JNIENV_CHECK_EXCEPTION();

    env->DeleteLocalRef(iarray);
    delete [] newdata;
    str = result.toString();
#endif

    return str;
}

QString DecodeWorker::decodeByZXingYUV(const QImage &img)
{
    if(img.size().isEmpty())
        return QString();

    QString str;

#ifdef Q_OS_ANDROID
    if(!m_zxingDecoder.isValid())
    {
        qDebug() << "DecodeWorker::decodeByZXingYUV(): decoder invalid";
        return QString();
    }
    const int width = img.width();
    const int height = img.height();
    const uchar *cImg =  img.constBits();
    const int size = width*height;

    // YUV格式每像素由3个字节存储，顺序存放Y-U-V分量
    const int bCnt = size*3; // QImage YUVLayout: QPixelFormat::YUV444
    char *yuv = new char[bCnt];
    for(int i = 0; i < size; i ++)
    {
        // RGB to YUV
        /**
         *
         * Y = 0.299R + 0.587G + 0.114B
         * U = -0.1687R - 0.3313G + 0.5B + 128
         * V = 0.5R - 0.4187G - 0.0813B + 128
         *
         * R = Y + 1.402(V-128)
         * G = Y - 0.34414(U-128) - 0.71414(V-128)
         * B = Y + 1.772(U-128)
         *
         * img format is: QImage::Format_RGBA8888
         * r: cImg[i*4 + 0]
         * g: cImg[i*4 + 1]
         * b: cImg[i*4 + 2]
         * a: cImg[i*4 + 3]
         *
         */
        char R = cImg[i*4 + 0];
        char G = cImg[i*4 + 1];
        char B = cImg[i*4 + 2];

        char Y = 0.299*R + 0.587*G + 0.144*B;
        char U = -0.1687*R - 0.3313*G + 0.5*B + 128;
        char V = 0.5*R - 0.4187*G - 0.0813*B + 128;

        yuv[i+0] = Y;
        yuv[i+1] = U;
        yuv[i+2] = V;
    }

    QAndroidJniEnvironment env;
    jbyteArray  byteArr = env->NewByteArray(bCnt);
    env->SetByteArrayRegion(byteArr, 0 , bCnt, (const jbyte*)yuv);

    QAndroidJniObject result = m_zxingDecoder.callObjectMethod("decodeYUVData", "([BII)Ljava/lang/String;", byteArr, width, height);

    JNIENV_CHECK_EXCEPTION();

    env->DeleteLocalRef(byteArr);
    delete [] yuv;
    str = result.toString();
#endif

    return str;
}

void DecodeWorker::setVibrate(bool vibrate)
{
    if(m_vibrate != vibrate)
    {
        m_vibrate = vibrate;
    }
}

bool DecodeWorker::isVibrate()
{
    return m_vibrate;
}

void DecodeWorker::onError(const QString &error)
{
    (void)error;
    //qDebug() << "DecodeWorker::onError():" << error;
}

void DecodeWorker::processImage(const QImage &img, int maxW, int maxH)
{
    bool success = false;
    QString text;

#ifdef Q_OS_ANDROID
    (void)maxW; (void)maxH;
    //text = decodeByZXingRGB(img);
    text = decodeByZXingYUV(img);
#else
    text = m_qzxingDecoder.decodeImage(img, maxW, maxH, true);
#endif

    if(text.isEmpty())
    {
        success = false;
    }
    else
    {
        //QString name = ns_getAppDataPath() + "/" + QTime::currentTime().toString("HHmmsszzz") + ".jpg";
        //img.save(name,"JPG",100);
        success = true;
#ifdef Q_OS_ANDROID
        if(m_vibrate)
        {
            ns_vibrate(200);
        }
#endif
    }

    emit decodeFinished(success, text);
}

DecodeImageEvent::DecodeImageEvent(): QEvent(g_eventType_decodeImage)
{
    m_width = -1;
    m_height = -1;
}

void DecodeImageEvent::setDecodeImage(const QImage &img, int maxW, int maxH)
{
    m_img = img;
    m_width = maxW;
    m_height = maxH;
}

QImage DecodeImageEvent::image() const
{
    return m_img;
}

int DecodeImageEvent::maxWidth() const
{
    return m_width;
}

int DecodeImageEvent::maxHeight() const
{
    return m_height;
}

VibrateEvent::VibrateEvent(): QEvent(g_eventType_vibrate)
{
    m_vibrate = true;
}

bool VibrateEvent::vibrate() const
{
    return m_vibrate;
}

void VibrateEvent::setVibrate(bool v)
{
    m_vibrate = v;
}
