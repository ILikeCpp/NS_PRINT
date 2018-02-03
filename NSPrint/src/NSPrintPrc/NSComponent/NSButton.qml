import QtQuick 2.5

Rectangle
{
    id: btn

    property alias text: desc.text;
    property alias textColor: desc.color;
    property alias textFontSize: desc.font.pixelSize;

    signal clicked();

    width: desc.width + 10 * pixelRate;
    height: desc.height + 6 * pixelRate;

    color: btnMouse.pressed ? "#4260A0" : "#388DCD";
    radius: 5 * pixelRate;

    opacity: btnMouse.pressed ? 0.6 : 1.0;

    NSText
    {
        id: desc;

        color: btn.enabled ? "white" : "gray";
        text: qsTr("button");
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent;

    }

    MouseArea
    {
        id: btnMouse

        anchors.fill: parent

        onClicked:
        {
            btn.clicked()
        }
    }
}
