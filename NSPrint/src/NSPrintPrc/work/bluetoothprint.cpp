#include "bluetoothprint.h"
#include <QDate>
#include <QCoreApplication>
#include <QDebug>
#include <QUuid>

#ifdef Q_OS_ANDROID

static const QEvent::Type g_print_eventType = static_cast<QEvent::Type>(QEvent::registerEventType());

/*******************************************************************/
void onStateChange_qt(JNIEnv *env, jobject thiz, jobject obj)
{
    QAndroidJniObject s = obj;
    BlueToothPrint::instance()->errorString(false,s.toString());
}

void onPrintProgress_qt(JNIEnv *env, jobject thiz, jobject obj)
{
    //打印失败怎么处理
    QAndroidJniObject s = obj;
    if(s.toString() == "Cancelled")//取消任务不通知。
    {
        return;
    }

    //打印标签成功，则继续打印下一个
    if(s.toString() == "Success")
    {
        BlueToothPrint::instance()->printNext();
        return;
    }

    BlueToothPrint::instance()->errorString(false, s.toString());
}

void onProgressInfo_qt(JNIEnv *env, jobject thiz, jobject obj)
{
    //蓝牙连接状态发生变化怎么处理
    QAndroidJniObject s = obj;
    BlueToothPrint::instance()->errorString(false, s.toString());
}

/*****************************************************************/
PrintEvent::PrintEvent(QEvent::Type type)
    :QEvent(type)
{
    this->setAccepted(false);
}

PrintEvent::~PrintEvent()
{

}

void PrintEvent::setData(QJsonObject obj)
{
    m_obj = obj;
}

QJsonObject PrintEvent::getData()
{
    return m_obj;
}

void PrintEvent::setEventType(PrintEvent::eventTypes type)
{
    m_type = type;
}

PrintEvent::eventTypes PrintEvent::getEventType()
{
    return m_type;
}

/***************************************************/
PrintWorker::PrintWorker(QObject *parent)
    :QObject(parent)
{
    m_localDevice = new QBluetoothLocalDevice(this);
    m_printer = QAndroidJniObject("an.qt.android.NSPrinter");
}

PrintWorker::~PrintWorker()
{

}

void PrintWorker::doPrint1(QJsonObject obj)
{
    //每次打印都判断有无打开蓝牙
    if(this->isBluetoothOPened())
    {
        //判断有无配对设备
        jboolean b = m_printer.callMethod<jboolean>("isPaired","()Z");
        if(b)
        {
            //每次打印都判断有无连接打印机,没有则连接
            jboolean isOpen =  m_printer.callMethod<jboolean>("openPrinter","()Z");
            if(!isOpen)
            {
                return;
            }
            //执行打印操作
            //所需参数 变电站名称 变电站装置名称 设备识别码 电力公司名称 打印日期
            QString bdzmc = obj.value("CZMC").toString().trimmed();
            QString zzmc = obj.value("YCSBMC").toString().trimmed();
            QString sbsbm = obj.value("SFSBM").toString().trimmed();
            QString dlgsmc = obj.value("gldw").toString().trimmed();
            QString dyrq = QString("打印日期：") + obj.value("date").toString().trimmed();//QDate::currentDate().toString("yyyy-MM-dd");
//            prepareParams(bdzmc,zzmc,sbsbm,dlgsmc,dyrq);

            QAndroidJniObject bdzmc_ = QAndroidJniObject::fromString(bdzmc);
            QAndroidJniObject zzmc_ = QAndroidJniObject::fromString(zzmc);
            QAndroidJniObject sbsbm_ = QAndroidJniObject::fromString(sbsbm);
            QAndroidJniObject dlgsmc_ = QAndroidJniObject::fromString(dlgsmc);
            QAndroidJniObject dyrq_ = QAndroidJniObject::fromString(dyrq);
            m_printer.callMethod<void>("printQRCode1","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",\
                                       bdzmc_.object<jstring>(),zzmc_.object<jstring>()\
                                       ,sbsbm_.object<jstring>(),dlgsmc_.object<jstring>(),dyrq_.object<jstring>());
        }
        else
        {
            emit errorString(false,"没有配对任何打印机");
        }
    }
    else
    {
        emit errorString(false,"蓝牙未打开");
    }
}

void PrintWorker::doPrint2(QJsonArray array)
{
    if(this->isBluetoothOPened())
    {
        jboolean b = m_printer.callMethod<jboolean>("isPaired","()Z");
        if(b)
        {
            jboolean isOpen =  m_printer.callMethod<jboolean>("openPrinter","()Z");
            if(!isOpen)
            {
                return;
            }
            QJsonObject obj1 = array.at(0).toObject();
            //所需参数 二维码 电力公司名称 打印日期
            QString code1 = obj1.value("code").toString().trimmed();
            QString dlgsmc1 = obj1.value("gldw").toString().trimmed();
            QString dyrq1 = QString("打印日期：") + obj1.value("date").toString().trimmed();

            QJsonObject obj2 = array.at(1).toObject();

            QString code2 = obj2.value("code").toString().trimmed();
            QString dlgsmc2 = obj2.value("gldw").toString().trimmed();
            QString dyrq2 = QString("打印日期：") + obj2.value("date").toString().trimmed();

            QAndroidJniObject code_1 = QAndroidJniObject::fromString(code1);
            QAndroidJniObject dlgsmc_1 = QAndroidJniObject::fromString(dlgsmc1);
            QAndroidJniObject dyrq_1 = QAndroidJniObject::fromString(dyrq1);

            QAndroidJniObject code_2 = QAndroidJniObject::fromString(code2);
            QAndroidJniObject dlgsmc_2 = QAndroidJniObject::fromString(dlgsmc2);
            QAndroidJniObject dyrq_2 = QAndroidJniObject::fromString(dyrq2);

            m_printer.callMethod<void>("printQRCode22","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",\
                                       code_1.object<jstring>(),dlgsmc_1.object<jstring>(),dyrq_1.object<jstring>(),code_2.object<jstring>(),dlgsmc_2.object<jstring>(),dyrq_2.object<jstring>());
        }
        else
        {
            emit errorString(false,"没有配对任何打印机");
        }
    }
    else
    {
        emit errorString(false,"蓝牙未打开");
    }
}

void PrintWorker::doExit()
{
    m_printer.callMethod<void>("printerQuit","()V");
    emit errorString(true,"退出打印");
}

QString PrintWorker::getPrinter()
{
    QAndroidJniObject name = m_printer.callObjectMethod("getPrinterName","()Ljava/lang/String;");
    return name.toString();
}

void PrintWorker::close()
{
    m_printer.callMethod<void>("closePrinter","()V");
    emit errorString(true,"关闭打印机连接");
}

void PrintWorker::doCancel()
{
    m_printer.callMethod<void>("cancel","()V");
    emit errorString(true,"取消成功");
}

bool PrintWorker::isBluetoothOPened()
{
    if( m_localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff )
    {
        //直接请求打开蓝牙
        m_localDevice->powerOn();
        return false;
    }
    return true;
}

void PrintWorker::prepareParams(QString& bdzmc, QString& zzmc,QString& sbsbm,QString& dlgsmc, QString& dyrq)
{
    bdzmc = "于祥变电站-2号所用变57362--以下测试文字长度";
    zzmc = "于祥变电站35KV2号所用变57362NS3697变压器保护设备";
    sbsbm = "29YC000000001547";
    dlgsmc = "国网南京电力公司江宁供电公司";
    dyrq = QString("打印日期：") + QDate::currentDate().toString("yyyy-MM-dd");
}

bool PrintWorker::event(QEvent *e)
{
    if(g_print_eventType == e->type())//接收到蓝牙打印事件
    {
        PrintEvent *event = static_cast<PrintEvent *>(e);
        if(NULL != event)
        {
            switch(event->getEventType())
            {
            case PrintEvent::print1:
            {
                doPrint1(event->getData());
            }
                break;
            case PrintEvent::print2:
            {
                doPrint2(event->getData().value("array").toArray());
            }
                break;
            case PrintEvent::cancel:
            {
                doCancel();
            }
                break;
            }

            return true;
        }
        return false;
    }
    return QObject::event(e);
}

/***************************************************/
PrintThread::PrintThread(QObject *parent)
    :QThread(parent)
{
    m_worker = nullptr;
}

PrintThread::~PrintThread()
{
    qDebug() << "~PrintThread";
}

void PrintThread::setData(PrintEvent::eventTypes type, QJsonObject obj)
{
    switch(type)
    {
    case PrintEvent::print1:
        this->msleep(1000);
        break;
    case PrintEvent::print2:
        this->msleep(1000);
        break;
    case PrintEvent::cancel:
        this->msleep(100);
        break;
    }
    PrintEvent *event = new PrintEvent(g_print_eventType);
    event->setData(obj);
    event->setEventType(type);
    QCoreApplication::postEvent(m_worker,event);
}

void PrintThread::run()
{
    m_worker = new PrintWorker();
    connect(m_worker,&PrintWorker::errorString,this,&PrintThread::errorString);

    this->exec();
    delete m_worker;
    m_worker = nullptr;
}
#endif
/************************************************************/
BlueToothPrint::Garbo BlueToothPrint::garbo;
BlueToothPrint *BlueToothPrint::m_instance = nullptr;

BlueToothPrint * BlueToothPrint::instance()
{
    if(nullptr == m_instance)
    {
        m_instance = new BlueToothPrint();
    }
    return m_instance;
}


BlueToothPrint::BlueToothPrint(QObject * parent)
    :QObject(parent)
{
#ifdef Q_OS_ANDROID
    m_thread = new PrintThread();
    connect(m_thread,&PrintThread::errorString,this,&BlueToothPrint::errorString);
    registerNativeMethods();
#endif
    m_seq = 0;
}

bool BlueToothPrint::registerNativeMethods()
{
    bool result = false;

#ifdef Q_OS_ANDROID
    JNINativeMethod methods []
    {
        {"onStateChange_qt","(Ljava/lang/String;)V",(void*)onStateChange_qt},
        {"onPrintProgress_qt","(Ljava/lang/String;)V",(void*)onPrintProgress_qt},
        {"onProgressInfo_qt","(Ljava/lang/String;)V",(void*)onProgressInfo_qt}
    };

    const char* classname = "an/qt/android/NSPrinter";
    jclass clazz;
    QAndroidJniEnvironment env;

//    clazz = env->FindClass(classname);

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());

    if(clazz)
    {
        jint ret = env->RegisterNatives(clazz,methods,\
                   sizeof(methods)/sizeof(methods[0]));
        env->DeleteLocalRef(clazz);
        result = ret >= 0;
    }

    if(env->ExceptionCheck())
    {
        env->ExceptionClear();
    }

#endif

    return result;
}

BlueToothPrint::~BlueToothPrint()
{
    qDebug() << "~BlueToothPrint";
#ifdef Q_OS_ANDROID
    if(nullptr != m_thread)
    {
        if(m_thread->isRunning())
        {
            m_thread->exit();
            m_thread->wait();
        }
        delete m_thread;
        m_thread = nullptr;
    }
#endif
}

void BlueToothPrint::requestPrintCustom(QJsonObject obj, int count)
{
    m_type = PrintType::type2;

#ifdef Q_OS_WIN

    QJsonArray container;

    if(count > 0 && !obj.isEmpty())
    {
        for(int i = 0; i < count; ++i)
        {
            //每一次uuid都不一样
            QJsonObject tmp = obj;
            QString code = tmp.value("code").toString().trimmed();
            code += QUuid::createUuid().toString();
            code.remove("{").remove("}").remove("-");
            tmp.insert("code",code);
            container.append(tmp);
            if(container.size() == 2)
            {
                QJsonObject obj;
                obj.insert("array",container);
                m_array.append(obj);
                container = QJsonArray();
            }
        }

        if(container.size() > 0)
        {
            container.append(QJsonObject());
            QJsonObject obj;
            obj.insert("array",container);
            m_array.append(obj);
        }

        qDebug() << "array = " << m_array;
    }

#endif

#ifdef Q_OS_ANDROID

    if(!m_thread->isRunning())
    {
        m_thread->start();
    }

    m_seq = 0;
    m_array = QJsonArray();//清空
    QJsonArray container;

    if(count > 0 && !obj.isEmpty())
    {
        for(int i = 0; i < count; ++i)
        {
            //每一次uuid都不一样
            QJsonObject tmp = obj;
            QString code = tmp.value("code").toString().trimmed();
            code += QUuid::createUuid().toString();
            code.remove("{").remove("}").remove("-");
            tmp.insert("code",code);
            container.append(tmp);
            if(container.size() == 2)
            {
                QJsonObject obj;
                obj.insert("array",container);
                m_array.append(obj);
                container = QJsonArray();
            }
        }

        if(container.size() > 0)
        {
            container.append(QJsonObject());
            QJsonObject obj;
            obj.insert("array",container);
            m_array.append(obj);
        }

        printNext();
    }
    else
    {
        emit errorString(false,"参数为空");
    }

#endif
}

void BlueToothPrint::requestPrint(QJsonArray array)
{
    m_type = PrintType::type1;
    qDebug() << "array:" << array;
#ifdef Q_OS_ANDROID
    if(!m_thread->isRunning())
    {
        m_thread->start();
    }

    m_seq = 0;//重置清零

    if(array.size() > 0)
    {
        m_array = array;
        this->printNext();
    }
    else
    {
        emit errorString(false,"参数为空");
    }
#endif
}

void BlueToothPrint::requestCancel()
{
    m_array = QJsonArray();
    m_seq = 0;

#ifdef Q_OS_ANDROID
    m_thread->setData(PrintEvent::cancel);
#endif
}

void BlueToothPrint::requestContinue()
{
    qDebug() << m_array << " " << m_seq;
    if(m_array.isEmpty() || m_seq == 0)
    {
        return;
    }

    m_seq -= 1;//回滚一次

    printNext();//继续打印下一个
}

void BlueToothPrint::printNext()
{
#ifdef Q_OS_ANDROID
    if(0 != m_seq)
    {
        switch(m_type)
        {
        case PrintType::type1:
            emit currentPrintIndex(m_seq);
            break;
        case PrintType::type2:
            emit currentPrintIndex(m_seq*2);
            break;
        }
    }

    if(m_seq == m_array.size())
    {
        emit errorString(true,"打印完成");
        return;
    }

    if(m_array.size() > m_seq)
    {
        QJsonObject obj = m_array.at(m_seq).toObject();
        switch(m_type)
        {
        case PrintType::type1:
            m_thread->setData(PrintEvent::print1,obj);
            break;
        case PrintType::type2:
            m_thread->setData(PrintEvent::print2,obj);
            break;
        }

        m_seq++;
    }
#endif
}
