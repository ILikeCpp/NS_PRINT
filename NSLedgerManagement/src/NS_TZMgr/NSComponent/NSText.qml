import QtQuick 2.5

Text
{
    //    font.family: "MicroSoft YaHei";
    //    font.family: "Helvetica";
    readonly property int defaultSize: 11 * pixelRate;

    font.pixelSize: 0

    Component.onCompleted:
    {
        if(Qt.platform.os === "windows")
        {
            font.family = "MicroSoft YaHei";
        }
    }
}
