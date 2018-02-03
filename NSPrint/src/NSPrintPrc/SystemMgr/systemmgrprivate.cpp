#include "systemmgrprivate.h"
#include <QDebug>

SystemMgrPrivate::SystemMgrPrivate(QObject *parent):
    QObject(parent)
{

}

SystemMgrPrivate::~SystemMgrPrivate()
{
    qDebug() << "~SystemMgrPrivate";
}

BlueToothPrint * SystemMgrPrivate::getBlueToothPrint()
{
    return BlueToothPrint::instance();
}
