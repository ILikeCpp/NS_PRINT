import QtQuick 2.0

Rectangle
{
    id: fillCoverMouse

    signal myClicked()

    anchors.fill: parent
    visible: false
    enabled: visible
    z: 0.5

    color: "#55999999"
    MouseArea
    {

        anchors.fill: parent
        propagateComposedEvents : true

        onClicked:
        {
            mouse.accepted = true
        }
        onPressAndHold:
        {
            mouse.accepted = true
        }
        onPressed:
        {
            mouse.accepted = true
        }
        onReleased:
        {
            fillCoverMouse.myClicked()
            mouse.accepted = true
        }
        onWheel: {
            wheel.accepted = true;
        }
    }
}
