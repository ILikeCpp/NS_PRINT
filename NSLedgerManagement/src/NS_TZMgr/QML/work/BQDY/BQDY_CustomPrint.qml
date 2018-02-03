import QtQuick 2.6
import "qrc:/NSComponent/NSComponent"

Item
{
    id:root;
    property string title: qsTr("标签打印");
    property bool showSearch: false;
    property bool showImport: false;
    property string gldw:"";
    property string date:"";

    function back()
    {
        return true;
    }

    function initData()
    {
        input1.text = "";
        input2.text = "";
    }

    function toPrint(res)
    {
        if(res)
        {
            jdShow.show();
            var obj = new Object;
            obj.gldw = root.gldw;
            obj.date = root.date;
            obj.code = input1.text.trim();
            root_blueToothPrint.requestPrintCustom(obj,parseInt(jdShow.total));
        }
    }

    //两个输入框
    Column
    {
        id:col;
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
            desText: qsTr("--请输入运维单位的代码--")
//            showImg: false;
            radius: 0;
        }

        NSInput
        {
            id:input2;
            width: parent.width;
            height: 40*pixelRate;
            desText: qsTr("--请输入需要打印标签的个数--")
//            showImg: false;
            radius: 0;
            validate: RegExpValidator { regExp: /[0-9]+/ }
        }
    }

    //下面是打印按钮
    Rectangle
    {
        id:btnItem;
        width: parent.width;
        height: printbtn.height + 10*pixelRate
        anchors.bottom: parent.bottom;
        color: "lightgray";

        NSButton
        {
            id:printbtn;
            text: qsTr("打印");
            width: parent.width/4;
            anchors.centerIn: parent;

            onClicked:
            {
                if(input1.text.trim() === "" || input1.text.length !== 4)
                {
                    topMessage.showMessage(qsTr("运维单位代码不能为空或长度不是4"))
                    return;
                }
                var pattern = /^\d{2}[A-Z]{2}$/;
                if(!pattern.test(input1.text.trim()))
                {
                    topMessage.showMessage(qsTr("运维单位代码不合法"));
                    return;
                }

                if(input2.text === "" || input2.text === "0")
                {
                    topMessage.showMessage(qsTr("标签个数不能为空或0"))
                    return;
                }

                var count = input2.text;//标签个数
                jdShow.total = count;
                var secs = count * 2;//耗时

                var hour = parseInt(secs / 3600);//获得小时
                var min = parseInt((secs % 3600) / 60);//获得分钟
                var sec = parseInt((secs % 3600) % 60);//获得秒数

                var str = qsTr("共需打印%1个标签,预计耗时%2小时%3分%4秒,是否确定打印?").arg(count).arg(hour).arg(min).arg(sec);
                messageBox.show(str,toPrint,qsTr("消息提示"));
            }
        }
    }

}
