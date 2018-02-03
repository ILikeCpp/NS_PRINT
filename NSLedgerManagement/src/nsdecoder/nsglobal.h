#ifndef NSGLOBAL_H
#define NSGLOBAL_H

#include <QObject>
#include <QDir>
#include "qqml.h"

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif


// 注册的qml包名
#ifndef NS_QML_PACKAGE
#define NS_QML_PACKAGE "NSSysPackage"
#endif

// QML主版本号
#ifndef NS_QML_MAJOR_VER
#define NS_QML_MAJOR_VER    1
#endif

// QML小版本号
#ifndef NS_QML_MINOR_VER
#define NS_QML_MINOR_VER    0
#endif

//数据库名称
#define NS_DB_NAME "tzmgr.db"

//数据库类型
#define NS_DB_TYPE "QSQLITE"

//数据库连接名称
#define NS_DB_CONNECTION_NAME "ns_sql_connection"

//数据库表名
#define NS_TABLE_NAME "bhzz" //保护装置表
#define NS_TABLE2_NAME "bhzzfz" //保护装置表辅助表
#define NS_TABLE2_COLNAME "colname"
#define NS_TABLE3_NAME "ysbhzz" //原始保护装置表
#define NS_TABLE4_NAME "fzbh" //辅助保护表
#define NS_TABLE5_NAME "ysfzbh" //辅助保护表
#define NS_TABLE6_NAME "fzbhfz" //辅助保护表辅助表
#define NS_TABLE6_COLNAME "colname"

//数据库中数据的状态
#define DATA_STATUS_ORIGINAL "DATA_STATUS_ORIGINAL" //初始
#define DATA_STATUS_UPDATE "DATA_STATUS_UPDATE" //修改更新
#define DATA_STATUS_DELETE "DATA_STATUS_DELETE" //删除
#define DATA_STATUS_INSERT "DATA_STATUS_INSERT" //插入

// Check JNI exception and clear it:
#define JNIENV_CHECK_EXCEPTION() \
    do{ \
    QAndroidJniEnvironment env; \
    if(env->ExceptionCheck()) \
{ \
    env->ExceptionClear(); \
    } \
    }while(0)


// 获取程序的数据文件夹路径，
// 在android中是外部存储目录中的NSData/NSClient
// 在windows中是相对程序运行路径的../data
inline QString ns_getAppDataPath()
{
    QString path;

#ifdef Q_OS_ANDROID
    QString mainDirectory("NSData");//主目录
    QString subDirectory("NSClient");//子目录

    //get variable directories of Android System
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                "android/os/Environment",
                "getExternalStorageDirectory",
                "()Ljava/io/File;"
                );
    JNIENV_CHECK_EXCEPTION();

    const QString external = externalStorageDir.toString();

    path = external  + "/" + mainDirectory + "/" + subDirectory;

    QDir dir;
    dir.setPath(path);

    if(dir.exists())
        return path;

    dir.setPath(external);

    if(!dir.exists())
        return QString();

    // make the path:
    bool ok = false;

    if(!dir.exists(mainDirectory))
    {
        ok = dir.mkdir(mainDirectory);
        if(!ok)
            return QString();
    }

    ok = dir.cd(mainDirectory);
    if(!ok)
    {
        return QString();
    }

    ok = dir.mkdir(subDirectory);
    if(!ok)
    {
        return QString();
    }
#endif

#ifdef Q_OS_WIN
    path = "../data";
#endif
    return path;
}


//获取软件所在盘或存储卡的根目录
inline QString ns_getRootDir()
{
#ifdef Q_OS_WIN
    return ns_getAppDataPath();
#endif
#ifdef Q_OS_ANDROID
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                "android/os/Environment",
                "getExternalStorageDirectory",
                "()Ljava/io/File;"
                );
    JNIENV_CHECK_EXCEPTION();

    QString external = externalStorageDir.toString();
    return external;
#endif
}

#endif // NSGLOBAL_H
