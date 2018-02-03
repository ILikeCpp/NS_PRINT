#include "systemmgr.h"
#include "systemmgrprivate.h"
#include <QDebug>

SystemMgr *SystemMgr::Instance()
{
    static SystemMgr mgr;
    return &mgr;
}

SystemMgr::SystemMgr(QObject *parent)
    :QObject(parent),d_ptr(new SystemMgrPrivate(this))
{

}

SystemMgr::~SystemMgr()
{
    qDebug() << "~SystemMgr";
}

BlueToothPrint * SystemMgr::getBlueToothPrint()
{
    return d_ptr->getBlueToothPrint();
}
