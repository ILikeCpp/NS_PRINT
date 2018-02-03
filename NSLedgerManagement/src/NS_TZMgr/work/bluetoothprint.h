#ifndef BLUETOOTHPRINT_H
#define BLUETOOTHPRINT_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

#ifdef Q_OS_ANDROID
#include <QThread>
#include <QEvent>
//蓝牙用到的头文件
#include <QBluetoothLocalDevice>

//Android相关类
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <jni.h>

class PrintEvent : public QEvent
{
public:
    enum eventTypes
    {
        print1,//打印1
        print2,//打印2
        cancel,//取消打印
    };
     explicit PrintEvent(QEvent::Type type);
    ~PrintEvent();

    void setData(QJsonObject obj);
    QJsonObject getData();
    void setEventType(PrintEvent::eventTypes type);
    PrintEvent::eventTypes getEventType();
private:
    QJsonObject m_obj;
    PrintEvent::eventTypes m_type;
};

class PrintWorker: public QObject
{
    Q_OBJECT
public:
    explicit PrintWorker(QObject *parent = 0);
    ~PrintWorker();

    bool event(QEvent *e);

signals:
    void errorString(bool result, QString error);

private:
    void doPrint1(QJsonObject obj);//执行打印
    void doPrint2(QJsonArray array);//执行打印
    void doExit();//退出打印
    void close();//关闭打印机连接
    void doCancel();//取消打印任务
    QString getPrinter();

    bool isBluetoothOPened();
    void prepareParams(QString& bdzmc, QString& zzmc,QString& sbsbm,QString& dlgsmc, QString& dyrq);

private:
    QBluetoothLocalDevice *m_localDevice;//本地设备
    QAndroidJniObject m_printer;//java对象
};

class PrintThread : public QThread
{
    Q_OBJECT
public:
    explicit PrintThread(QObject *parent = 0);
    ~PrintThread();

    void setData(PrintEvent::eventTypes type, QJsonObject obj = QJsonObject());

signals:
    void errorString(bool result, QString error);

protected:
    void run();
private:
    PrintWorker *m_worker;
};
#endif

class BlueToothPrint : public QObject
{
    Q_OBJECT

public:

    enum PrintType
    {
        type1,
        type2
    };

    static BlueToothPrint *instance();

    //打印
    Q_INVOKABLE void requestPrint(QJsonArray array);
    Q_INVOKABLE void requestPrintCustom(QJsonObject obj, int count);
    Q_INVOKABLE void requestCancel();
    Q_INVOKABLE void requestContinue();
    void printNext();

    bool registerNativeMethods();

    class Garbo//自动释放
    {
    public:
        ~Garbo()
        {
            if (BlueToothPrint::m_instance)
            {
                delete BlueToothPrint::m_instance;
            }
        }
    };
    static Garbo garbo;

signals:
    void errorString(bool result, QString error);
    void currentPrintIndex(const qint32 index);
private://单例
    explicit BlueToothPrint(QObject * parent = 0);
    ~BlueToothPrint();
    BlueToothPrint(const BlueToothPrint& that);//禁止拷贝构造
    BlueToothPrint& operator =(const BlueToothPrint& that);//禁止拷贝赋值
private:
    static BlueToothPrint * m_instance;
    QString m_msg;//界面消息通知
#ifdef Q_OS_ANDROID
    PrintThread *m_thread;
#endif
    QJsonArray m_array;
    qint32 m_seq;//打印项数
    PrintType m_type;
};

#endif // BLUETOOTHPRINT_H
