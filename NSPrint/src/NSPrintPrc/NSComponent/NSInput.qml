import QtQuick 2.5
import "qrc:/image/source/image/"

Rectangle
{
    property alias text: input.text;
    property alias echoMode: input.echoMode;
    property alias desText: description.text;
    property alias edit: input.focus;
    property alias validate: input.validator;

    width: 100 * pixelRate;
    height: input.height + 10 * pixelRate;
    border.color: enabled ? "darkgray" : "darkgray";
    border.width: 1;
    radius: 5 * pixelRate;

    NSText
    {
        id: description;

        text: "";
        visible: input.length === 0;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: parent.left;
        anchors.leftMargin: 5 * pixelRate;

        color: "lightgray";

    }
    TextInput
    {
        id: input;

        anchors.left: parent.left;
        anchors.leftMargin: 10 * pixelRate;
        anchors.verticalCenter: parent.verticalCenter;

        width: parent.width - 20 * pixelRate;
        clip: true;
        verticalAlignment: TextInput.AlignVCenter;

        color: enabled ? "black" : "gray";

        font.family: "MicroSoft YaHei";
        font.pixelSize: 14 * pixelRate;

        passwordMaskDelay: 500;
        selectByMouse: true;
        onAccepted:
        {
            focus = false;
        }
    }
}

