import QtQuick 2.6
import "qrc:/NSComponent/NSComponent"

Rectangle
{
    id: mainForm;

    property string title: qsTr("标签打印");
    property var printForm: null;

    Component.onDestruction:
    {
        if(printForm !== null)
        {
            printForm.destroy();
        }
    }

    color: "#006567"

    function back()
    {
        return true;
    }

    //进入标签打印界面
    function moveToPrint()
    {
        if(printForm === null)
        {
            var com = Qt.createComponent("qrc:/work/QML/work/BQDY/BQDY_Mgr.qml");
            if(com.status === Component.Ready)
            {
                printForm = com.createObject();
            }
            else
            {
                console.debug("标签打印界面创建失败:", com.errorString());
            }

            com.destroy();
        }

        if(printForm !== null)
        {
            mainStackView.push(printForm);
        }
    }

    //界面
    Image
    {
        id:bg;
        source: "qrc:/image/source/image/home/矩形-1.png";
        width: mainForm.width/4;
        height: width;
        anchors.centerIn: parent;
        opacity: mos.pressed? 0.5 : 1;

        MouseArea
        {
            id:mos;
            anchors.fill: parent;
            onClicked:
            {
                moveToPrint();
            }
        }

        Column
        {
            anchors.centerIn: parent;
            spacing: 5*pixelRate;
            Image
            {
                id:bgs;
                source: "qrc:/image/source/image/home/打印.png";
                sourceSize: Qt.size(40*pixelRate,40*pixelRate);
            }

            NSText
            {
                text: qsTr("标签打印")
                width: parent.width;
                horizontalAlignment: Text.AlignHCenter;
                color: "white";
            }
        }
    }

    Image
    {
        id:logoimg;
        source: "qrc:/image/source/image/home/logo.png";
        sourceSize: Qt.size(parent.width/2,parent.width/2);
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: parent.height/10;
        anchors.horizontalCenter: parent.horizontalCenter;
    }
}

