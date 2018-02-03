import QtQuick 2.5
/*菜单组件:例如点击软件主界面的右上角的菜单按钮时候显示出来的下拉菜单*/
Rectangle
{
    id: menu;

    width: 0;
    height: column.height;
    z: 1;

    property var items: [];
    property var mouseCover: null;

    signal signal_result(int id); //返回的信号

    visible: false;
    enabled: visible;

    //清除列表中的对象
    function clear()
    {
        while(items.length > 0)
        {
            items[0].destroy();
            items.splice(0,1);
        }
     }

    //清除列表中的某个对象
    function removeItemAt(index)
    {
        items[index].destroy();
        items.splice(index,1);
    }

    //新建并添加某个对象到列表中去
    function addItem(text, id)//按顺序，从上往下添加,要写个index来弄吗？
    {
        var obj = itemCom.createObject(column)
        obj.name = text
        obj.id = id
        obj.index = menu.items.length;

        var wAdd = 80; // 宽度比文字宽度增加

        //应该对宽度高度有一个比较调整
        if(menu.height > 0)
        {
            if(obj.textWidth + wAdd * pixelRate > menu.width)
            {
                menu.width = obj.textWidth + wAdd * pixelRate
                //所有的子对象的宽也变化的说
            }
        }
        else
        {
            menu.width = obj.textWidth + wAdd* pixelRate
        }
        items.push(obj)

        return obj
    }

    //获取列表中的总个数
    function itemsCount()
    {
        return items.length;
    }

    //关闭并销毁
    function close()
    {
        clear();
        if(mouseCover !== null)
        {
            mouseCover.visible = false;
        }
        menu.destroy();
    }

    //关闭隐藏
    function hide()
    {
        if(visible === false)
            return;

        visible = false;

        if(mouseCover !== null)
        {
            mouseCover.visible = false;
        }
    }

    //显示
    function show()
    {
        visible = true;
        if(mouseCover !== null)
        {
            mouseCover.visible = true;
        }
    }

    //单击menu菜单以外的地方
    function noActionClicked()
    {
        signal_result(-1);
    }

    Component.onCompleted:
    {
        var com = Qt.createComponent("qrc:/NSComponent/NSComponent/WholeCover.qml");
        if(com.status === Component.Ready)
        {
            mouseCover = com.createObject(parent);
            mouseCover.myClicked.connect(noActionClicked);
        }
        else
        {
            console.debug(qsTr("错误："), com.errorString());
        }
        com.destroy();
    }

    Component.onDestruction:
    {
        clear();
        if(mouseCover !== null)
        {
            mouseCover.myClicked.disconnect(noActionClicked);
            mouseCover.destroy();
        }
    }

    Column
    {
        id: column;

        anchors.fill: parent;
    }

    Component
    {
        id: itemCom

        Rectangle
        {
            id: item

            property int index: -1;
            property alias name: itemText.text;
            property alias textWidth: itemText.contentWidth;
            property int id: -1;

            height: itemText.contentHeight + 15*pixelRate;
            width: parent.width;

            opacity: itemMouse.pressed ? 0.5 : 1.0;
            color: itemMouse.pressed ? "lightgray" : "#FFFFFF";

            visible: item.id === 4 || item.id === 5 ? stackView.currentItem === homeForm : true;

            clip: true;

            NSText
            {
                id: itemText;

                anchors.centerIn: parent;
            }

            MouseArea
            {
                id: itemMouse;

                anchors.fill: parent;

                onClicked:
                {
                    signal_result(item.id);
                }
            }

            Rectangle
            {
                id: line;

                height: 1;
                width: parent.width;

                color: item.index === menu.items.length-1 ? "transparent" : "darkgray";

                anchors
                {
                    horizontalCenter: parent.horizontalCenter;
                    bottom: parent.bottom;
                }
            }
        }
    }
}

