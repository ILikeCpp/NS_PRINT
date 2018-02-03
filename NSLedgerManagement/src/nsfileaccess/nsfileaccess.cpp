#include "nsfileaccess.h"
#include <QDir>
#include <QFileInfo>

//#define DEBUG_FILEACCESS
//#ifdef DEBUG_FILEACCESS
#include <QDebug>
//#endif

class NSFileAccessPrivate
{
public:
    NSFileAccessPrivate(NSFileAccess *q);

    void setFilters(int filters);
    void setSortFlags(int sortFlags);
    void setNameFilters(const QStringList &filters);

    QDir::Filters toQDirFilters(int filters) const;
    QDir::SortFlags toQDirSortFlags(int sortFlags) const;
public:
    NSFileAccess * const q_ptr;
    QDir m_dir;
    QStringList m_nameFilters;
    int m_filters;
    int m_sorts;
};

NSFileAccessPrivate::NSFileAccessPrivate(NSFileAccess *q):
    q_ptr(q),
    m_filters(NSFileAccess::EF_NoFilter),
    m_sorts(NSFileAccess::SF_NoSort)
{
    setFilters(NSFileAccess::EF_NoFilter);
    setSortFlags(NSFileAccess::SF_NoSort);
}

void NSFileAccessPrivate::setFilters(int filters)
{
    m_filters = filters;
    QDir::Filters qFilters = toQDirFilters(filters);

    m_dir.setFilter(qFilters);
}

void NSFileAccessPrivate::setSortFlags(int sortFlags)
{
    m_sorts = sortFlags;
    QDir::SortFlags sorts = toQDirSortFlags(sortFlags);

    m_dir.setSorting(sorts);
}

void NSFileAccessPrivate::setNameFilters(const QStringList &filters)
{
    m_nameFilters = filters;
    m_dir.setNameFilters(filters);
}

QDir::Filters NSFileAccessPrivate::toQDirFilters(int filters) const
{
    QDir::Filters qFilters = 0;

    if(filters & NSFileAccess::EF_Dirs) {
        qFilters |= QDir::Dirs;
    }
    if(filters & NSFileAccess::EF_Drives) {
        qFilters |= QDir::Drives;
    }
    if(filters & NSFileAccess::EF_Files) {
        qFilters |= QDir::Files;
    }
    if(filters & NSFileAccess::EF_NoDot) {
        qFilters |= QDir::NoDot;
    }
    if(filters & NSFileAccess::EF_NoDotDot) {
        qFilters |= QDir::NoDotDot;
    }

    return qFilters;
}

QDir::SortFlags NSFileAccessPrivate::toQDirSortFlags(int sortFlags) const
{
    QDir::SortFlags sorts = 0;

    if(sortFlags & NSFileAccess::SF_DirsFirst) {
        sorts |= QDir::DirsFirst;
    }
    if(sortFlags & NSFileAccess::SF_DirsLast) {
        sorts |= QDir::DirsLast;
    }
    if(sortFlags & NSFileAccess::SF_IgnoreCase) {
        sorts |= QDir::IgnoreCase;
    }
    if(sortFlags & NSFileAccess::SF_Name) {
        sorts |= QDir::Name;
    }
    if(sortFlags & NSFileAccess::SF_Reversed) {
        sorts |= QDir::Reversed;
    }
    if(sortFlags & NSFileAccess::SF_Size) {
        sorts |= QDir::Size;
    }
    if(sortFlags & NSFileAccess::SF_Time) {
        sorts |= QDir::Time;
    }
    if(sortFlags & NSFileAccess::SF_Type) {
        sorts |= QDir::Type;
    }

    return sorts;
}

NSFileAccess::NSFileAccess(QObject *parent):
    QObject(parent),
    d_ptr(new NSFileAccessPrivate(this))
{
}

NSFileAccess::~NSFileAccess()
{
    delete d_ptr;
}

bool NSFileAccess::exists(const QString &path)
{
    return d_ptr->m_dir.exists(path);
}

void NSFileAccess::setLocalPath(const QString &path)
{
    d_ptr->m_dir.setPath(path);
}

QString NSFileAccess::getLocalPath() const
{
    return d_ptr->m_dir.absolutePath();
}

bool NSFileAccess::localCdUp()
{
    if(d_ptr->m_dir.path().isEmpty())
        return false;

    bool ok = false;
    if(d_ptr->m_dir.isRoot())
    {
        d_ptr->m_dir.setPath("");
        ok = true;
    }
    else
    {
        ok = d_ptr->m_dir.cdUp();
    }

    return ok;
}

bool NSFileAccess::localCd(const QString &dir)
{
    return d_ptr->m_dir.cd(dir);
}

void NSFileAccess::localCdRoot()
{
    d_ptr->m_dir.setPath("");
}

QList<NSFileAccess::EntityInfo> NSFileAccess::localEntityList() const
{
    QList<EntityInfo> eInfoList;
    QList<QFileInfo> fileInfos;

    if(d_ptr->m_dir.absolutePath().isEmpty())
    {
        if(d_ptr->m_filters & ET_Drive)
        {
            fileInfos = d_ptr->m_dir.drives();
            for(int i = 0; i < fileInfos.size(); ++i)
            {
                QFileInfo info = fileInfos.at(i);
                EntityInfo eInfo;
                eInfo.m_type = ET_Drive;
                eInfo.m_location = EL_Local;
                eInfo.m_name = info.filePath();
                eInfo.m_path = info.filePath();
                eInfo.m_isHidden = info.isHidden();
                eInfo.m_lastModified = info.lastModified();

                eInfoList.append(eInfo);
            }
        }
    }
    else
    {
        fileInfos = d_ptr->m_dir.entryInfoList();
        for(int i = 0; i < fileInfos.size(); ++i)
        {
            QFileInfo info = fileInfos.at(i);
            EntityInfo eInfo;

            if(info.isDir()) {
                eInfo.m_type = ET_Directory;
            }
            else if(info.isFile()) {
                eInfo.m_type = ET_File;
                eInfo.m_size = info.size();
            }

            eInfo.m_location = EL_Local;
            eInfo.m_name = info.fileName();
            eInfo.m_path = info.absoluteFilePath();
            eInfo.m_isHidden = info.isHidden();
            eInfo.m_lastModified = info.lastModified();

            eInfoList.append(eInfo);
        }
    }

    return eInfoList;
}

QList<NSFileAccess::EntityInfo> NSFileAccess::localEntityList(const QStringList &dirNameFilters, const QStringList &fileNameFilters, int dirFilters, int fileFilters, int sorting) const
{
    QList<EntityInfo> eInfoList;
    QList<QFileInfo> fileInfos;

    if(d_ptr->m_dir.absolutePath().isEmpty())
    {
        fileInfos = d_ptr->m_dir.drives();
        for(int i = 0; i < fileInfos.size(); ++i)
        {
            QFileInfo info = fileInfos.at(i);
            EntityInfo eInfo;
            eInfo.m_type = ET_Drive;
            eInfo.m_location = EL_Local;
            eInfo.m_name = info.absolutePath();
            eInfo.m_path = info.filePath();
            eInfo.m_isHidden = info.isHidden();
            eInfo.m_lastModified = info.lastModified();

            eInfoList.append(eInfo);
        }
    }
    else
    {
        QDir::Filters qFilters = QDir::NoFilter;
        QDir::SortFlags qSortFlag = d_ptr->toQDirSortFlags(sorting);

        // directory:
        qFilters = d_ptr->toQDirFilters(dirFilters);
        fileInfos = d_ptr->m_dir.entryInfoList(dirNameFilters, qFilters, qSortFlag);
        for(int i = 0; i < fileInfos.size(); ++i)
        {
            QFileInfo info = fileInfos.at(i);
            EntityInfo eInfo;
            eInfo.m_type = ET_Directory;
            eInfo.m_location = EL_Local;
            eInfo.m_name = info.fileName();
            eInfo.m_path = info.absoluteFilePath();
            eInfo.m_isHidden = info.isHidden();
            eInfo.m_lastModified = info.lastModified();

            eInfoList.append(eInfo);
        }

        // files:
        qFilters = d_ptr->toQDirFilters(fileFilters);
        fileInfos = d_ptr->m_dir.entryInfoList(fileNameFilters, QDir::Files, qSortFlag);
        for(int i = 0; i < fileInfos.size(); ++i)
        {
            QFileInfo info = fileInfos.at(i);
            EntityInfo eInfo;
            eInfo.m_type = ET_File;
            eInfo.m_location = EL_Local;
            eInfo.m_name = info.fileName();
            eInfo.m_path = info.absoluteFilePath();
            eInfo.m_isHidden = info.isHidden();
            eInfo.m_lastModified = info.lastModified();
            eInfo.m_size = info.size();

            eInfoList.append(eInfo);
        }
    }

    return eInfoList;
}

QStringList NSFileAccess::nameFilters() const
{
    return d_ptr->m_nameFilters;
}

void NSFileAccess::setNameFilters(const QStringList &filters)
{
    d_ptr->setNameFilters(filters);
}

int NSFileAccess::sortFlags() const
{
    return d_ptr->m_sorts;
}

void NSFileAccess::setSortFlags(int sort)
{
    d_ptr->setSortFlags(sort);
}

int NSFileAccess::filter() const
{
    return d_ptr->m_filters;
}

void NSFileAccess::setFilter(int filters)
{
    d_ptr->setFilters(filters);
}
