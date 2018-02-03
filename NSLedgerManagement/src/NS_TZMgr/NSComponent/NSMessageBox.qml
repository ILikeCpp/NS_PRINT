import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/NSComponent/NSComponent"

//确认的消息框！
WholeCover{
    id: messageBox;
    property var myFunction: null;
    signal onClicked(bool result);
    Rectangle{
        id: messageRec1
        width: parent.width - 40 * pixelRate;
        color: "white";
        radius: 5 * pixelRate;
        anchors.centerIn: parent;
        height: header.height + contentRec.height;
        scale: 0;
        states:[
            State{
                name: "show"
                PropertyChanges {
                    target: messageRec1;
                    scale: 1;
                }
            }
        ]
        transitions: [
            Transition {
                from: "show"
                to: ""
                NumberAnimation{
                    property: "scale";
                    duration: 0;
                }
                onRunningChanged: {
                    if(running === false)
                    {
                        messageBox.visible = false;
                    }
                }
            },
            Transition {
                from: ""
                to: "show"
                NumberAnimation{
                    property: "scale";
                    duration: 100 ;
                }
            }
        ]
        MouseArea
        {
            anchors.fill: parent;
        }

        Item{
            id: header;
            width: parent.width;
            height: headerTitle.contentHeight + 15 * pixelRate;
            NSText{
                id: headerTitle;
                width: parent.width - 20 * pixelRate;
                anchors.centerIn: parent;
                color: "#7F99AC";
                elide: Text.ElideRight;
            }
        }
        Rectangle{
            id: contentRec;
            width: parent.width;
            height: messageTitle.height + msgBtnRec.height;
            anchors.top: header.bottom;
            radius: 5 * pixelRate;
            color: "#F0F1F5";
            Rectangle{
                color: "#F0F1F5";
                height: 5 * pixelRate;
                width: parent.width;
            }

            NSText{
                id: messageTitle;
                height: contentHeight + 20 * pixelRate
                width: parent.width - 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                wrapMode: Text.WrapAnywhere;
                text: qsTr("确认退出？");
                font.pixelSize: 12 * pixelRate;
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter;
            }
            Item{
                id: msgBtnRec;
                width: parent.width;
                height: msgOkBtn.height + 15 * pixelRate;
                anchors.top: messageTitle.bottom;
                NSButton{
                    id: msgOkBtn;
                    text: qsTr("确认");
                    color: "#1BA4AA";
                    textColor: "white";
                    border.width: 0;
                    width: parent.width / 3;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.left: parent.left;
                    anchors.leftMargin: parent.width / 9;
                    onClicked: {
                        messageBox.hide();
                        messageBox.onClicked(true);
                        messageBox.myFunction(true)
                    }
                }
                NSButton{
                    id:btn1;
                    text: qsTr("取消");
                    color: "#547590";
                    textColor: "white";
                    border.width: 0
                    width: parent.width / 3;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.right: parent.right;
                    anchors.rightMargin: parent.width / 9;
                    onClicked: {
                        messageBox.hide();
                        messageBox.onClicked(false);
                        messageBox.myFunction(false);
                    }
                }
            }
        }
    }

    onMyClicked: {
        messageBox.hide();
//        messageBox.onClicked(false);
//        myFunction(false);
    }

    function back()
    {
        messageBox.hide();
//        messageBox.onClicked(false);
//        myFunction(false);
    }


    function hide()
    {
        messageRec1.state = "";
    }

    function show(message,xb_function,title)
    {
        if(xb_function !== null && xb_function !== undefined)
        {
            myFunction = xb_function;
            msgOkBtn.text = qsTr("确定");
            btn1.text = qsTr("取消");
            messageTitle.text = message;
            messageBox.visible = true;
            messageRec1.state = "show";
        }
        if(title === undefined)
        {
            headerTitle.text = "提示";
        }
        else
        {
            headerTitle.text = title;
        }
    }

    function show1(message,xb_function,title,btn1name,btn2name)
    {
        if(xb_function !== null && xb_function !== undefined)
        {
            myFunction = xb_function;
            msgOkBtn.text = btn1name;
            btn1.text = btn2name;
            messageTitle.text = message;
            messageBox.visible = true;
            messageRec1.state = "show";
        }
        if(title === undefined)
        {
            headerTitle.text = "提示";
        }
        else
        {
            headerTitle.text = title;
        }
    }
}
