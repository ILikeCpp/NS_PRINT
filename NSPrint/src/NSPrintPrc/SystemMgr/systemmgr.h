//总的管理类 向外提供接口
#ifndef SYSTEMMGR_H
#define SYSTEMMGR_H

#include <QObject>
#include "systemmgrprivate.h"

class SystemMgr : public QObject
{
    Q_OBJECT
public:
    static SystemMgr *Instance();

    BlueToothPrint * getBlueToothPrint();

private:
    explicit SystemMgr(QObject *parent = 0);
    ~SystemMgr();
    SystemMgr(const SystemMgr& that);//禁止拷贝赋值
    SystemMgr& operator =(const SystemMgr& that);//禁止赋值

    SystemMgrPrivate *d_ptr;
};

#endif // SYSTEMMGR_H
