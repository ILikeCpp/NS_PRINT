#ifndef NSFILEACCESS_H
#define NSFILEACCESS_H

#include <QObject>
#include <QList>
#include <QDateTime>


#if defined(NSFILEACCESS_LIBRARY)
#  define NSFILEACCESSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NSFILEACCESSSHARED_EXPORT Q_DECL_IMPORT
#endif

class NSFileAccessPrivate;
class NSFILEACCESSSHARED_EXPORT NSFileAccess: public QObject
{
    Q_OBJECT
public:
    enum EntityLocation
    {
        EL_Unknow,
        EL_Online,
        EL_Local
    };
    Q_ENUM(EntityLocation)

    enum EntityType
    {
        ET_Unknow,
        ET_Drive,
        ET_Directory,
        ET_File
    };
    Q_ENUM(EntityType)

    enum EntityFilters
    {
        EF_NoFilter         = -1,
        EF_Dirs             = 0x001,
        EF_Files            = 0x002,
        EF_Drives           = 0x004,
        EF_AllEntity        = EF_Dirs | EF_Files | EF_Drives,
        EF_NoDot            = 0x008,
        EF_NoDotDot         = 0x010,
        EF_NoDotAndDotDot   = EF_NoDot | EF_NoDotDot
    };

    enum SortFlags
    {
        SF_NoSort           = -1,
        SF_Name             = 0x01,
        SF_Time             = 0x02,
        SF_Size             = 0x04,
        SF_Type             = 0x08,
        SF_DirsFirst        = 0x10,
        SF_DirsLast         = 0x20,
        SF_Reversed         = 0x40,
        SF_IgnoreCase       = 0x80
    };

    struct EntityInfo
    {
        EntityLocation m_location;
        EntityType m_type;
        QString m_name;
        QString m_path;
        qint64 m_size;
        bool m_isHidden;
        QDateTime m_lastModified;

        EntityInfo(): m_location(EL_Unknow), m_type(ET_Unknow),
            m_size(0), m_isHidden(false) {}
    };

public:
    explicit NSFileAccess(QObject *parent = 0);
    ~NSFileAccess();

    // online:

    // local:
    bool exists(const QString &path);
    void setLocalPath(const QString &path);
    QString getLocalPath() const;
    bool localCdUp();
    bool localCd(const QString &dir);
    void localCdRoot();
    QList<NSFileAccess::EntityInfo> localEntityList() const;
    QList<NSFileAccess::EntityInfo> localEntityList(const QStringList &dirNameFilters, const QStringList &fileNameFilters, int dirFilters, int fileFilters, int sorting) const;

    // common setting:
    QStringList nameFilters() const;
    void setNameFilters(const QStringList &filters);

    int sortFlags() const;
    void setSortFlags(int sort);

    int filter() const;
    void setFilter(int filters);

private:
    NSFileAccessPrivate * const d_ptr;
};

#endif // NSFILEACCESS_H
