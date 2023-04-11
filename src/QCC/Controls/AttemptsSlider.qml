import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Slider {
    id: slider
    width: 100; height: font.pixelSize * 0.4
    orientation: Qt.Horizontal
    snapMode: Slider.SnapAlways
    stepSize: 1

    topPadding: 0
    bottomPadding: 0
    padding: 0

    live: false
    handle: Item {}
    enabled: false

    property alias border: background.border
    property alias color: background.color

    property color dashColor: "white"
    property alias selectedColor: selectRec.color

    background: Rectangle {
        id: background
        x: slider.leftPadding
        y: slider.topPadding// + slider.availableHeight / 2 - height / 2
        width: slider.availableWidth
        height: slider.availableHeight  // slider.implicitHeight
        radius: 2
        color: Material.hintTextColor

        Rectangle {
            id: selectRec
            width: slider.visualPosition * parent.width
            height: parent.height
            color: Material.accent
            radius: 2
        }

        Repeater {
            id: repeater
            model: (slider.to - slider.from) / slider.stepSize - 1

            Rectangle {
                height: parent.height; width: 1;
                color: slider.dashColor
                x: (index + 1) * (slider.availableWidth / (repeater.count + 1))
            }
        }
    }
}



