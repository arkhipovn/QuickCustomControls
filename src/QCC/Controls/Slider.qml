import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Item {
    width: 400
    height: repeaterTitles.height + slider.height + slider.anchors.topMargin;

    property alias hoverEnabled: slider.hoverEnabled

    property var values: []
    property alias currentIndex: slider.value
    property alias pressed: slider.pressed

    property alias sliderItem: slider

    Repeater {
        id: repeaterTitles
        model: values

        Label {
            id: text
            text: modelData
            y: repeaterTitles.height
            x: {
                var px = index * ((slider.availableWidth - slider.handle.width) / (repeaterTitles.count - 1))
                if(px < slider.leftPadding) px = slider.leftPadding
                else if(px > (slider.width - slider.rightPadding - paintedWidth)) px = slider.width - slider.rightPadding - paintedWidth
                return px
            }

            opacity: slider.enabled ? 1.0 : 0.3
            color: index == slider.value ? Material.accent : Material.foreground
        }
    }

    Slider {
        id: slider
        anchors { top: repeaterTitles.bottom; topMargin: font.pixelSize * 1.5; }
        width: parent.width
        snapMode: Slider.SnapAlways
        from: 0
        to: values.length - 1
        stepSize: 1

        background: Rectangle {
            id: backgroundRec
            x: slider.leftPadding
            y: slider.topPadding + slider.availableHeight / 2 - height / 2

            width: slider.availableWidth
            height: font.pixelSize * 0.7
            radius: 2

            color: Material.hintTextColor//Material.shade(Material.foreground, Material.Shade100)

            Repeater {
                model: values
                height: parent.height

                Rectangle {
                    color: backgroundRec.color
                    width: 2; height: parent.height * 1.7
                    anchors.verticalCenter: parent.verticalCenter
                    x: slider.handle.width * 0.5 + index * ((slider.availableWidth - slider.handle.width) / (slider.to - slider.from))
                }
            }
        }

        //        handle: Rectangle {
        //            x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
        //            y: slider.topPadding + slider.availableHeight / 2 - height / 2
        //            implicitWidth: font.pixelSize * 2.2; implicitHeight: font.pixelSize * 2.2; radius: width / 2
        //            color: slider.pressed ? Material.foreground : Material.accent   //Theme.mainColor
        //            border.color: "#bdbebf"
        //        }
    }


}
