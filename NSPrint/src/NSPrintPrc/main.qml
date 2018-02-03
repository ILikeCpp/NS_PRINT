import QtQuick 2.5
import QtQuick.Window 2.2

Window                                                 //主窗体
{
    id: mainWindow;

    visible: true;

    width: 320 * pixelRate;
    height: 569 * pixelRate;

    property var contentObj: null;

    Component.onCompleted:
    {
        initForm();
    }

    function initForm()
    {
        var com = Qt.createComponent("qrc:/work/QML/work/HomeMain.qml");
        if(com.status === Component.Ready)
        {
            contentObj = com.createObject(cont, {"focus": true});
        }
        else
        {
            console.debug("error", com.errorString());
        }
        com.destroy();
    }

    Item
    {
        id: cont;
        anchors.fill: parent;
    }
}

