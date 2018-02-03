import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/NSComponent/NSComponent"

Item
{
    id:root;
    property string title: qsTr("标签打印");
    property bool showSearch: false;
    property bool showImport: false;

    property var printForm: null;
    property var autoprint: null;

    Component.onDestruction:
    {
        if(printForm !== null)
        {
            printForm.destroy();
        }

        if(autoprint !== null)
        {
            autoprint.destroy();
        }
    }

    function back()
    {
        return true;
    }

    function moveToPrintQRCodeByDev(gldw,date)
    {
        if(printForm === null)
        {
            var com = Qt.createComponent("qrc:/work/QML/work/BQDY/BQDY_CZList.qml");
            if(com.status === Component.Ready)
            {
                printForm = com.createObject();
            }
            else
            {
                console.debug("error", com.errorString());
            }

            com.destroy();
        }

        if(printForm !== null)
        {
            mainStackView.push(printForm);
            printForm.gldw = gldw;
            printForm.date = date;
            printForm.initData();
        }
    }

    function moveToPrintQRCodeByAuto(gldw,date)
    {
        if(autoprint === null)
        {
            var com = Qt.createComponent("qrc:/work/QML/work/BQDY/BQDY_CustomPrint.qml");
            if(com.status === Component.Ready)
            {
                autoprint = com.createObject();
            }
            else
            {
                console.debug("error", com.errorString());
            }

            com.destroy();
        }

        if(autoprint !== null)
        {
            mainStackView.push(autoprint);
            autoprint.gldw = gldw;
            autoprint.date = date;
            autoprint.initData();
        }
    }


    Column
    {
        id: col;
        width: parent.width*4/5
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.top: parent.top;
        anchors.topMargin: parent.height/5;
        spacing: 30*pixelRate;

        NSInput
        {
            id:input1;
            width: parent.width;
            height: 40*pixelRate;
            desText: qsTr("--请输入管理单位--")
            radius: 0;
        }

        NSInput
        {
            id:input2;
            width: parent.width;
            height: 40*pixelRate;
            text: Date.now().toLocaleString();
            desText: qsTr("--请输入打印时间--")
            radius: 0;
        }

        Column
        {
            id: resultCol;
            width: parent.width;
            spacing: 10 * pixelRate;
            clip: true;

            ExclusiveGroup {id:exc}
            RadioButton
            {
                id:leftBtn;
                checked: true;
                exclusiveGroup: exc;
                style: RadioButtonStyle {
                    indicator: Rectangle {
                        implicitWidth: 16*pixelRate
                        implicitHeight: 16*pixelRate
                        radius: 8*pixelRate
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 1
                        Rectangle {
                            anchors.fill: parent
                            visible: control.checked
                            color: "#388DCD"
                            radius: 8*pixelRate
                            anchors.margins: 4*pixelRate
                        }
                    }

                    label: NSText
                    {
                        text: qsTr("根据设备识别码生成二维码标签打印");
                        font.pixelSize: 12*pixelRate;
                    }
                    spacing: 5*pixelRate;
                }
            }

            RadioButton
            {
                id:rightBtn;
                checked: false;
                exclusiveGroup: exc;
                style: RadioButtonStyle {
                    indicator: Rectangle {
                        implicitWidth: 16*pixelRate
                        implicitHeight: 16*pixelRate
                        radius: 8*pixelRate
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 1
                        Rectangle {
                            anchors.fill: parent
                            visible: control.checked
                            color: "#388DCD"
                            radius: 8*pixelRate
                            anchors.margins: 4*pixelRate
                        }
                    }

                    label: NSText
                    {
                        text: qsTr("自动生成二维码标签打印")
                        font.pixelSize: 12*pixelRate;
                    }
                    spacing: 5*pixelRate;
                }
            }
        }
    }

    Rectangle
    {
        id:btnItem;
        width: parent.width;
        height: btn1.height + 10*pixelRate;
        anchors.bottom: parent.bottom;
        color: "lightgray"
        NSButton
        {
            id:btn1;
            text: qsTr("退出");
            width: parent.width/4;
            anchors.left: parent.left;
            anchors.leftMargin: 30*pixelRate
            anchors.verticalCenter: parent.verticalCenter;
            onClicked:
            {
                mainStackView.pop();
            }
        }

        NSButton
        {
            id:btn2;
            text: qsTr("打印");
            width: parent.width/4;
            anchors.right: parent.right;
            anchors.rightMargin: 30*pixelRate;
            anchors.verticalCenter: parent.verticalCenter;
            onClicked:
            {
                if(input1.text.trim() === "")
                {
                    topMessage.showMessage(qsTr("管理单位不能为空"));
                    return;
                }

                if(input2.text.trim() === "")
                {
                    topMessage.showMessage(qsTr("日期不能为空"));
                    return;
                }

                var pattern = /^((?:19|20)\d\d)-(0[1-9]|1[012])-(0[1-9]|[12][0-9]|3[01])$/;
                if(!pattern.test(input2.text.trim()))
                {
                    topMessage.showMessage(qsTr("日期格式不合法"));
                    return;
                }

                if(leftBtn.checked)
                {
                    moveToPrintQRCodeByDev(input1.text.trim(),input2.text.trim());
                }
                else if(rightBtn.checked)
                {
                    moveToPrintQRCodeByAuto(input1.text.trim(),input2.text.trim());
                }
            }
        }
    }
}
