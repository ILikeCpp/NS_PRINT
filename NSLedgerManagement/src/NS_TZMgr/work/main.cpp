#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QFont>
#include <QQmlContext>

#include "SystemMgr/systemmgr.h"

//获取分辨率
float pixelRate()
{
    QScreen *screen = QGuiApplication::primaryScreen();

    QFont appFont = QGuiApplication::font();
    int pixelSize = 12;
    if(appFont.pixelSize() > 0)
    {
        pixelSize = appFont.pixelSize();
    }
    else if(appFont.pointSize() > 0)
    {
        pixelSize = (appFont.pointSize() * screen->logicalDotsPerInch()) / 72;
    }

    return pixelSize/12.0;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SystemMgr *sys = SystemMgr::Instance();

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    if(context)
    {
        context->setContextProperty("pixelRate", pixelRate());
        context->setContextProperty("root_blueToothPrint",sys->getBlueToothPrint());
    }

    engine.load(QUrl(QStringLiteral("qrc:/work/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
