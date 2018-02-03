// 退出确认界面

import QtQuick 2.5
Item{
    id: message

    property alias text: messageText.text
    property var btnObjList: []//按钮对象

    property var mouseCover: null

    signal btnClicked(string name, var obj)

    enabled: false
    visible: enabled

    z: 1

    implicitWidth: messageContent.width - 1
    implicitHeight: messageContent.height


    //覆盖在全界面的鼠标事件响应
    function coverMouse()
    {
        btnClicked("", message)
    }

    //关闭并销毁对象
    function close()
    {
        if(mouseCover !== null)
        {
            mouseCover.visible = false;
        }

        animSmall.start();
        message.destroy();
    }

    //隐藏消息对象，不销毁
    function hide()
    {
        if(animSmall.running === true || visible === false)
            return;

        if(mouseCover != null)
        {
            mouseCover.visible = false;
        }

        animSmall.start();
    }

    //显示消息对象
    function show()
    {  resetwidthheight()
        message.anchors.centerIn = parent;
         enabled = true;

        animBig.start();
        if(mouseCover != null)
        {
            mouseCover.visible = true;
        }
    }

    //为消息添加按钮，并或得添加的按钮对象，text按钮显示的名字，name按钮标记的名字（注意唯一性，这里没有检查）
       function addBtn(text, name)//添加一个按钮操作
       {
           var obj = btnComponet.createObject(row)
           obj.text = text
           obj.name = name

           btnObjList.push(obj)

           return obj//这个算是返回的按钮吗
       }
       //根据文字长度，按钮长度设定按钮的宽以及文字的宽
       function resetwidthheight()
       {
           if(btnObjList.length === 0)
           {
               return
           }

           //检测宽度是否一样
           var width = btnObjList[0].width
           var index
           for(index = 1; index < btnObjList.length; index++)
           {
               if(width < btnObjList[index].width)
               {
                   width = btnObjList[index].width
               }
           }
           //按钮的宽度和那个宽度做比较
           if(width * btnObjList.length < messageRec.width)
               width = messageRec.width/btnObjList.length
           else
           {
               messageRec.width = width * btnObjList.length
           }

           //找出了最长的宽度，然后统一宽度
           for(index = 0; index < btnObjList.length; index++)
           {
               btnObjList[index].width = width
           }

           row.width = width * btnObjList.length
           row.height = btnObjList[0].height
       }
       //移动对象
       MouseArea
       {
           anchors.fill: parent

           drag.target: message  // root可拖动
           drag.axis: Drag.XAndYAxis

           onPressed: {
               message.anchors.centerIn = undefined;
           }
       }


    //消息框，包括文字和按钮
       Rectangle
       {
           id: messageContent

           anchors.centerIn: parent

           width: messageRec.width
           height: messageRec.height + row.height + 2
           radius: 5 * pixelRate;
           color: "#EEEFF3";

           Rectangle//文字部分，文字过长的话怎么办？
           {
               id: messageRec

               width: messageText.width + 20 * pixelRate
               height: messageText.height + 20 * pixelRate
               radius: 5 * pixelRate;
               anchors.top: parent.top
               //            anchors.left: parent.left
               anchors.horizontalCenter: parent.horizontalCenter;

               //            color: "transparent"
               Text
               {
                   id: messageText

                   font.pixelSize: 14 * pixelRate;
                   font.family: "Microsoft YaHei"
                   color: "#328571";
                   anchors.centerIn: parent;
               }
               Rectangle{
                   color: "white";
                   width: parent.width;
                   height: 5 * pixelRate;
                   anchors.bottom: parent.bottom;
               }
           }

           Row//按钮部分
           {
               id: row

               anchors.bottom: parent.bottom
               anchors.horizontalCenter: parent.horizontalCenter
           }
       }


       //按钮组件
       Component
       {
           id: btnComponet

           Item
           {
               id: btn

               property alias text: btnText.text //显示的按钮名字
               property string name:  ""//id标记，会先自动分配一个唯一的name吧？

               clip: true

               height: Math.max(btnText.height*2, 40 * pixelRate)
               width: btnText.width + 50 * pixelRate;
               //            border.color: "#c4bbbb"

               //            implicitWidth: btnText.width * 3 / 2 //一般情况下应该所有的按钮宽度相同的说，每一次都要比较？
               Rectangle
               {
                   width: btnText.width + 20 * pixelRate;
                   height: btnText.height + 5 * pixelRate;
                   anchors.centerIn: parent;
                   radius: 3 * pixelRate;
                   color: btnText.text == "取 消" ? "#7A7879" : "#328571";

                   Text
                   {
                       id: btnText
                       anchors.centerIn: parent
                       color: "white";
                       font.family: "Microsoft Yahei";
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       font.pixelSize: 14 * pixelRate;
                   }

                   MouseArea
                   {
                       id: messageMouse

                       anchors.fill: parent;

                       onClicked:
                       {
                           btnClicked(name, message)//name就没有必要了吧，倒是放入根对象，外部决定是否要析构

                       }
                   }
               }
           }
       }


    // size（如何控制size动画的中心点）
    //动画的实现
    PropertyAnimation {
        id: animBig;
        target: message;
        duration: 200;
//        easing.type: Easing.OutBounce
        property: 'scale';
        from: 0;
        to: 1;
    }
    PropertyAnimation {
        id: animSmall;
        target: message;
        duration: 200;
//        easing.type: Easing.OutBounce
        property: 'scale';
        from: 1;
        to: 0;

        onStopped:
        {
            message.enabled = false;
        }
    }


    Component.onCompleted:
    {
        var com = Qt.createComponent("qrc:/NSComponent/NSComponent/WholeCover.qml");
        if(com.status === Component.Ready)
        {
            mouseCover = com.createObject(parent);
            mouseCover.myClicked.connect(coverMouse);
        }
        else
        {
            console.debug(qsTr("错误："), com.errorString());
        }
        com.destroy();
    }

    //消息对象销毁的时候
    Component.onDestruction:
    {
        if(mouseCover !== null)
        {
            mouseCover.myClicked.disconnect(coverMouse);
            mouseCover.destroy();
        }
        //销毁按钮对象
        while(btnObjList.length > 0)
        {
            btnObjList.pop().destroy()
        }
    }

}
