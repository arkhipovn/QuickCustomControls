import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root
    property int frameCounter: 0
    property int counter: 0
    property int fps: 0

    color: "transparent"
    //    color: "black"
    height: Material.buttonHeight

    width: row.width + Material.buttonHeight

    Row {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height

        spacing: 5

        Rectangle {
            id: spinnerImage
            anchors.verticalCenter: parent.verticalCenter
            color: Material.color(Material.Teal)

            width: parent.height * 0.2; height: width; radius: 1

            NumberAnimation on rotation {
                from:0
                to: 360
                duration: 800
                loops: Animation.Infinite
            }
            onRotationChanged: frameCounter++;
        }

        Label {
            anchors.verticalCenter: spinnerImage.verticalCenter
            color: "#c0c0c0"
            text: root.fps + " fps"
        }
    }

    Timer {
        interval: 1000
        repeat: true
        running: true
        onTriggered: {
            root.fps = frameCounter// * 4 // тк интервал 250
            frameCounter = 0;
        }
    }
}
