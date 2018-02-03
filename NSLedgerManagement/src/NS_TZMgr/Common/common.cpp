#include "common.h"
#include "nsglobal.h"
#include <QtMath>
#include <QDebug>
#include <QDate>

#ifdef Q_OS_WIN
#include <QDesktopServices>
#endif

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

Common::Common(QObject *parent)
    :QObject(parent)
{
    m_fileAccess = nullptr;
}

Common::~Common()
{
    qDebug() << "~Common";
}

void Common::registerQMLType()
{
    qmlRegisterType<NSFileAccess>(NS_QML_PACKAGE, NS_QML_MAJOR_VER, NS_QML_MINOR_VER, "NSFileAccess");
}

QString Common::praseUrltofilePath(const QUrl url)
{
    return url.toLocalFile();
}

QString Common::getAppAbsolutePath()
{
    QDir dir(ns_getAppDataPath());
    return dir.absolutePath();
}

void Common::initFilePath()
{
    if(m_fileAccess == NULL)
    {
        m_fileAccess = new NSFileAccess(this);
    }

    QString path = ns_getAppDataPath();
    if(!QFile::exists(path))
    {
        QDir dir;
        dir.mkpath(path);
    }
    if(m_fileAccess != NULL)
    {
        m_fileAccess->setLocalPath(path);
        qDebug() << "path=" << path;
        m_fileAccess->setSortFlags(NSFileAccess::SF_DirsFirst|NSFileAccess::SF_IgnoreCase);
    }
}

void Common::initFilePath(QString path)
{
    if(m_fileAccess == NULL)
    {
        m_fileAccess = new NSFileAccess(this);
    }
    if(m_fileAccess != NULL)
    {
        m_fileAccess->setLocalPath(path);
        m_fileAccess->setSortFlags(NSFileAccess::SF_DirsFirst|NSFileAccess::SF_IgnoreCase);
    }
}

int Common::getPathCount()
{
    if(m_fileAccess == NULL)
        return 0;

    QStringList myFilters;
    myFilters<<"*.path";
    QStringList myDirFilters;
    myDirFilters<<"*";
    m_fileList = m_fileAccess->localEntityList(myDirFilters,myFilters,NSFileAccess::EF_Dirs|NSFileAccess::EF_Drives|NSFileAccess::EF_NoDotAndDotDot,NSFileAccess::EF_Files,NSFileAccess::SF_DirsFirst|NSFileAccess::SF_IgnoreCase);
    return m_fileList.count();
}

int Common::getFilesCount()
{
    if(m_fileAccess == NULL)
        return 0;
    QStringList filters;
    filters << "*.xlsx" << "*.*";
    QStringList pathFilters;
    m_fileList = m_fileAccess->localEntityList(pathFilters,filters,NSFileAccess::EF_Drives|NSFileAccess::EF_Dirs|NSFileAccess::EF_NoDotAndDotDot,QDir::Files,QDir::NoSort);

    return m_fileList.count();
}

QString Common::getFilePathAt(const int &index)
{
    if(index < 0 || index >= m_fileList.count())
    {
        return "";
    }

    return m_fileList.at(index).m_path;
}
QString Common::getFileNameAt(const int &index)
{
    if(index < 0 || index >= m_fileList.count())
    {
        return "";
    }

    return m_fileList.at(index).m_name;
}

QString Common::getIconAt(const int &index)
{
    if(index < 0 || index >= m_fileList.count())
    {
        return "";
    }

    int type = m_fileList.at(index).m_type;
    switch(type)
    {
    case  NSFileAccess::ET_Unknow:
        return "qrc:/image/source/image/file/file.png";
        break;

    case NSFileAccess::ET_Drive:
        return "qrc:/image/source/image/file/drive.png";
        break;
    case NSFileAccess::ET_Directory:
        return "qrc:/image/source/image/file/dir.png";
        break;
    case NSFileAccess::ET_File:
        QStringList icon;
        icon << "doc" << "docx" << "dwg" << "gif" << "jpg" << "pdf" << "png"<< "xlsx"<< "xls";
        QString suffix = m_fileList.at(index).m_name.split(".").last();
        if(icon.contains(suffix))
        {
            return "qrc:/image/source/image/file/"+suffix+"file.png";
        }
        break;
    }

    return "qrc:/image/source/image/file/file.png";

}

int Common::getTypeAt(const int &index)
{
    if(index < 0 || index >= m_fileList.count())
    {
        return NSFileAccess::ET_Unknow;
    }

    return m_fileList.at(index).m_type;
}

QString Common::getFileSize(const int &index)
{
    if(index < 0 || index >= m_fileList.count())
    {
        return 0;
    }
    qint64 size = m_fileList.at(index).m_size;
    return transformFileSize(size);
}

QStringList Common::getPathList(const QList<int> &indexList)
{
    QStringList list;

    foreach (int index, indexList)
    {
        if(index < 0 || index >= m_fileList.count())
        {
            continue;
        }
        list.append(m_fileList.at(index).m_path);
    }
    return list;
}

QString Common::getDirName()
{
    if(m_fileAccess == NULL)
    {
        return "";
    }
    return m_fileAccess->getLocalPath();
}

bool Common::cdUp()
{
    if(m_fileAccess == NULL)
        return false;

    return m_fileAccess->localCdUp();

}

bool Common::cd(const QString &name)
{
    if(m_fileAccess == NULL)
        return false;

    return m_fileAccess->localCd(name);

}

QString Common::transformFileSize(qint64 fileSize)
{
    if(fileSize/1024 == 0)
    {
        return QString("%1B").arg(fileSize);
    }
    else if(fileSize/1024/1024 == 0)
    {
        return QString("%1KB").arg(qCeil((double)fileSize/1024));
    }
    else if(fileSize/1024/1024/1024 == 0)
    {
        return QString("%1MB").arg(QString::number((double)fileSize/1024/1024, 'f', 2));

    }
    else
    {
        return QString("%1GB").arg(QString::number((double)fileSize/1024/1024/1024, 'f', 4));
    }
    return "0B";
}

QString Common::getFileNameByPath(QString filePath)
{
    QFileInfo info(filePath);
    if(info.exists())
    {
        return info.fileName();
    }
    return QString("");
}

int Common::getMyFilesCount()
{
    if(m_fileAccess == NULL)
        return 0;

    QStringList myDirFilters;
    myDirFilters<<"*";
    m_fileList = m_fileAccess->localEntityList(myDirFilters,m_filter,NSFileAccess::EF_Dirs|NSFileAccess::EF_Drives|NSFileAccess::EF_NoDotAndDotDot,NSFileAccess::EF_Files,NSFileAccess::SF_DirsFirst|NSFileAccess::SF_IgnoreCase);

    return m_fileList.count();
}

QString Common::getFileIcon(QString fileName)
{
    QStringList list = fileName.split(".");
    if(list.count() < 2)
        return "qrc:/image/source/image/file/file.png";

    QStringList icon;
    icon << "doc" << "docx" << "docx" << "dwg" << "file" << "gif" << "jpg" << "pdf"<<"png"<<"xls"<<"xlsx";
    if(icon.contains(list.last()))
    {
        return "qrc:/image/source/image/file/"+list.last()+"file.png";
    }
    return "qrc:/image/source/image/file/file.png";
}

bool Common::openFile(const QString filePath)
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject filepath = QAndroidJniObject::fromString(filePath);
    QAndroidJniObject intentAction = QAndroidJniObject::fromString("android.intent.action.VIEW");
    QAndroidJniObject intentCategory = QAndroidJniObject::fromString("android.intent.category.DEFAULT");

    // MIME type
    QAndroidJniObject mimeType = QAndroidJniObject::callStaticObjectMethod("java/net/URLConnection", "guessContentTypeFromName", "(Ljava/lang/String;)Ljava/lang/String;", filepath.object<jclass>());
    if(mimeType.toString().isEmpty())
    {
        mimeType = QAndroidJniObject::fromString("application/octet-stream");
    }

    QAndroidJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", intentAction.object<jstring>());

    QAndroidJniObject fileObj("java/io/File", "(Ljava/lang/String;)V", filepath.object<jstring>());

    // addCategory
    intent.callObjectMethod("addCategory", "(Ljava/lang/String;)Landroid/content/Intent;", intentCategory.object<jstring>());
    JNIENV_CHECK_EXCEPTION();

    // addFlags
    jint flagTask = intent.getField<jint>("FLAG_ACTIVITY_NEW_TASK");
    intent.callObjectMethod("addFlags", "(Ljava/lang/Integer;)Landroid/content/Intent;", flagTask);
    JNIENV_CHECK_EXCEPTION();

    QAndroidJniObject uriObj = QAndroidJniObject::callStaticObjectMethod("android/net/Uri", "fromFile", "(Ljava/io/File;)Landroid/net/Uri;", fileObj.object<jclass>());
    intent.callObjectMethod("setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;",uriObj.object<jclass>(), mimeType.object<jstring>());
    JNIENV_CHECK_EXCEPTION();

    QtAndroid::startActivity(intent, 0);

    return true;
#endif

#ifdef Q_OS_WIN
    return QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
#endif
}

QString Common::getCurrentDate()
{
    return QDate::currentDate().toString("yyyy-MM-dd");
}

bool Common::copyFile(QString sourceFile, QString destFile, bool coverFile)
{
    if(sourceFile.isEmpty() || destFile.isEmpty())
    {
        return false;
    }

    if(coverFile)
    {
        QFile::remove(destFile);
    }
    else
    {
        if(QFile::exists(destFile))
        {
            return false;
        }
    }

    if(!QFile::exists(sourceFile))
    {
        return false;
    }

    //执行拷贝
    if(!QFile::copy(sourceFile,destFile))
    {
        return false;
    }

    return true;
}

bool Common::copyDir(QString sourceDir, QString destDir, bool coverDir)
{

}
