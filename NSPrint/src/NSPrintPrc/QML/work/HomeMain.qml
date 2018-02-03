import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2
import "qrc:/NSComponent/NSComponent"

Item
{
    id: root;
    visible: true
    anchors.fill: parent;
    focus: true;
    property var quitDialog: null;

    Component.onDestruction:
    {
        if(quitDialog !== null)
        {
            quitDialog.btnClicked.disconnect(closeResult)
            quitDialog.destroy();
        }
    }

    Keys.enabled: true;

    Keys.onBackPressed: //这个是那个手机平板的返回键
    {
        back();
        if(mainStackView.depth <= 1)
        {
            closeCheck();
        }
    }

    Keys.onUpPressed: //电脑向上键
    {
        back();
        if(mainStackView.depth <= 1)
        {
            closeCheck();
        }
    }

    function back()
    {
        if(topMessage.visible)
        {
            topMessage.hide();
            return;
        }

        if(messageBox.visible)
        {
            messageBox.hide();
            return;
        }

        if(jdShow.visible)
        {
//            jdShow.hide();
            return;
        }

        mainStackView.back();
    }

    //退出程序相关的检测
    function closeCheck()
    {
        if(quitDialog  === null)
        {
            var com = Qt.createComponent("qrc:/NSComponent/NSComponent/QuitConfirmForm.qml");
            if(com.status === Component.Ready)
            {
                quitDialog = com.createObject(root);
                quitDialog.anchors.centerIn = root;

                quitDialog.text = qsTr("确定退出?")
                quitDialog.addBtn(qsTr("退 出"), qsTr("OK"))
                quitDialog.addBtn(qsTr("取 消"), qsTr("cancel"))
                quitDialog.btnClicked.connect(closeResult)
                quitDialog.show();
            }
            else
            {
                console.debug(qsTr("错误："), com.errorString());
            }
            com.destroy();
        }
        else
        {
            quitDialog.show();
        }
    }

    function closeResult(value)
    {
        switch(value)
        {
        case "OK":
            Qt.quit()
            break;
        default:
            quitDialog.hide()
            break
        }
    }

    //界面
    Rectangle
    {
        id: homeHeader
        width: parent.width;
        height: homeTitle.height + 20 * pixelRate;
        anchors.top: parent.top;

        gradient:Gradient
        {
            GradientStop { position: 0.0; color: "#49577C"; }
            GradientStop { position: 1.0; color: "#323C5E"; }
        }
        NSText{
            id: homeTitle;
            anchors.centerIn: parent;
            color: "white";
            text: qsTr("标签管理");
        }

        //左边返回图标
        Image
        {
            id:backimg;
            visible: mainStackView.depth > 1;
            anchors.left: parent.left;
            anchors.leftMargin: 10*pixelRate;
            anchors.verticalCenter: parent.verticalCenter;
            source: "qrc:/image/source/image/home/back.png";
            sourceSize: Qt.size(25*pixelRate,25*pixelRate);
            opacity: imgMos.pressed? 0.5 : 1;
            MouseArea
            {
                id:imgMos;
                anchors.fill: parent;
                onClicked:
                {
                    root.back();
                }
            }
        }

        //右边更多图标
        Image
        {
            id:moreBtn;
            anchors.right: parent.right;
            anchors.rightMargin: 10*pixelRate;
            anchors.verticalCenter: parent.verticalCenter;
            source: "qrc:/image/source/image/home/more.png";
            sourceSize: Qt.size(20*pixelRate,20*pixelRate);
            opacity: imgMos1.pressed? 0.5 : 1;
            MouseArea
            {
                id:imgMos1;
                anchors.fill: parent;
                onClicked:
                {
                    menu.updateMenu();
                }
            }
        }
    }


    StackView
    {
        id: mainStackView;
        anchors.top: homeHeader.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        function back()
        {
            if(mainStackView.currentItem.back())
            {
                mainStackView.pop();
            }
        }

        onCurrentItemChanged:
        {
            if(currentItem !== null)
            {
                currentItem.focus = true;
                homeTitle.text = Qt.binding(function(){return currentItem.title});
            }
            else
            {
                homeTitle.text = "";
            }
        }

        initialItem: MainForm {
            id: mainForm;
        }
    }

    NSShowMessage
    {
        id:topMessage;
    }

    NSMessageBox
    {
        id:messageBox;
    }

    NSMenu
    {
        id:menu;
        anchors.right: homeHeader.right;
        anchors.top: homeHeader.bottom;

        Component.onCompleted:
        {
            menu.signal_result.connect(menuItemClicked);
            menu.addItem(qsTr("关于"),0);
            menu.addItem(qsTr("退出"), 1);
        }

        Component.onDestruction:
        {
            menu.signal_result.disconnect(menuItemClicked);
        }

        function updateMenu()
        {
            menu.show();
        }

        function menuItemClicked(id)
        {
            menu.hide();
            switch(id)
            {
            case 0:
                break;
            case 1:
                closeCheck();
                break;
            }
        }
    }

    //打印进入显示框
    WholeCover
    {
        id:jdShow;
        property alias total: txt1.total;
        property alias finished: txt1.finished;
        property bool iserror: false;
        property bool isok: false;

        onMyClicked:{
        }

        function hide()
        {
            root_blueToothPrint.currentPrintIndex.disconnect(onCurrentPrintIndex);
            root_blueToothPrint.errorString.disconnect(onDoPrint);
            showmsg.text = "";
            iserror = false;
            isok = false;
            visible = false;
        }

        function show()
        {
            root_blueToothPrint.currentPrintIndex.connect(onCurrentPrintIndex);
            root_blueToothPrint.errorString.connect(onDoPrint);
            showmsg.text = "正在打印. . .";
            iserror = false;
            isok = false;
            visible = true;
        }

        function onCurrentPrintIndex(index)
        {
            if(index > jdShow.total)
            {
                jdShow.finished = jdShow.total;
            }
            else
            {
                jdShow.finished = index;
            }
        }

        function onDoPrint(result,msg)
        {
            if(result)
            {
                showmsg.text = msg;
                iserror = false;
                isok = true;
                timer.running = false;
            }
            else
            {
                showmsg.text = msg;
                iserror = true;
                isok = false;
            }
        }

        Rectangle
        {
            width: parent.width*3/4;
            anchors.top: parent.top;
            anchors.topMargin: parent.height/4;
            height: parent.height/2 - 30*pixelRate;
            radius: 8*pixelRate;
            anchors.horizontalCenter: parent.horizontalCenter;

            //显示打印任务和进度
            Column
            {
                id:jdcol;
                width: parent.width*2/3;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: parent.top;
                anchors.topMargin: 10*pixelRate
                spacing: 5*pixelRate;

                NSText
                {
                    id:txt1;
                    property int total: 0;
                    property int finished: 0;
                    width: parent.width;
                    text: qsTr("打 印 进 度: %1 / %2").arg(finished).arg(total)
                    wrapMode: Text.WrapAnywhere;
                }

                NSText
                {
                    id:showSec;
                    text: qsTr("打 印 耗 时: 00:00:00");
                    width: parent.width;
                    wrapMode: Text.WrapAnywhere;
                }

                Timer
                {
                    id:timer;
                    running: jdShow.visible;
                    repeat: true;
                    property int secs: -1;
                    onRunningChanged:
                    {
                        if(!running)
                        {
                            secs = -1;
                        }
                    }

                    onTriggered:
                    {
                        secs++;
                        setSpeakTimes();
                    }

                    function setSpeakTimes()
                    {
                        var hour = parseInt(secs / 3600);//获得小时
                        var min = parseInt((secs % 3600) / 60);//获得分钟
                        var sec = parseInt((secs % 3600) % 60);//获得秒数

                        if(parseInt(hour / 10) === 0)
                        {
                            hour = "0" + hour;
                        }
                        if(parseInt(min / 10) === 0)
                        {
                            min = "0" + min;
                        }
                        if(parseInt(sec / 10) === 0)
                        {
                            sec = "0" + sec;
                        }

                        showSec.text = qsTr("打 印 耗 时: %1:%2:%3").arg(hour).arg(min).arg(sec);
                    }
                }
            }

            Rectangle
            {
                id:sp1;
                width: parent.width;
                height: 5*pixelRate;
                color: "lightgray";
                anchors.top: jdcol.bottom;
                anchors.topMargin: 5*pixelRate;
            }


            Item
            {
                width: parent.width;
                anchors.top: sp1.bottom;
                anchors.topMargin: 5*pixelRate;
                anchors.bottom: sp2.top;
                anchors.bottomMargin: 5*pixelRate;
                //下面是错误信息显示
                NSText
                {
                    id:showmsg
                    color: "red";
                    width: parent.width*2/3
                    anchors.centerIn: parent;
                    wrapMode: Text.WrapAnywhere;
                    horizontalAlignment: Text.AlignHCenter;
                }
            }

            Rectangle
            {
                id:sp2;
                visible: jdShow.iserror || jdShow.isok;
                width: parent.width;
                height: 1
                color: "lightgray";
                anchors.bottom: jdbtn.top;
                anchors.bottomMargin: 5*pixelRate;
            }

            //底部两个按钮
            NSButton
            {
                id:jdbtn;
                text: qsTr("继续打印");
                width: parent.width/4
                visible: jdShow.iserror;
                anchors.bottom: parent.bottom;
                anchors.bottomMargin: 5*pixelRate;
                anchors.left: parent.left;
                anchors.leftMargin: 20*pixelRate;
                onClicked:
                {
                    root_blueToothPrint.requestContinue();
                }
            }

            NSButton
            {
                text: qsTr("取消打印");
                visible: jdShow.iserror;
                width: parent.width/4
                anchors.bottom: parent.bottom;
                anchors.bottomMargin: 5*pixelRate;
                anchors.right: parent.right;
                anchors.rightMargin: 20*pixelRate;
                onClicked:
                {
                    root_blueToothPrint.requestCancel();
                    jdShow.hide();
                }
            }

            NSButton
            {
                id:okBtn;
                text: qsTr("确定");
                visible: jdShow.isok;
                width: parent.width/4
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.bottom;
                anchors.bottomMargin: 5*pixelRate;
                onClicked:
                {
                    jdShow.hide();
                }
            }
        }
    }
}
