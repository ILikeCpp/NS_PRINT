#ifndef SYSTEMMGRPRIVATE_H
#define SYSTEMMGRPRIVATE_H

#include <QObject>
#include "work/bluetoothprint.h"

class SystemMgrPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SystemMgrPrivate(QObject *parent = 0);
    ~SystemMgrPrivate();

    BlueToothPrint * getBlueToothPrint();

private:
};

#endif // SYSTEMMGRPRIVATE_H
