import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import QtQuick.Layouts 1.12


ColumnLayout {
    id: control
    width: 400
    spacing: 0//Material.menuItemVerticalPadding

    property alias hoverEnabled: slider.hoverEnabled
    property alias pressed: slider.pressed
    property alias currentIndex: slider.value

    property var model: null

    property alias sliderHeight: backgroundRec.height

    signal moved()

    Slider {
        id: slider
        Layout.fillWidth: true
        snapMode: Slider.SnapAlways

        from: 0
        to: control.model == null ? 0
                                  : control.model.length === undefined ? control.model
                                                                       : control.model.length - 1
        stepSize: 1

        onMoved: control.moved()

        background: Rectangle {
            id: backgroundRec
            x: slider.leftPadding
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            width: slider.availableWidth; height: 4
            radius: 2
            color: Material.hintTextColor

            Repeater {
                model: control.model == null ? null
                                             : slider.to + 1

                Rectangle {
                    y: parent.height - 1
                    x: slider.handle.width * 0.5 + index * ((parent.width - slider.handle.width) / (slider.to - slider.from))
                    width: 2; height: parent.height  * 1.5
                    color: backgroundRec.color
                }
            }
        }
    }   // Slider

    RowLayout {
        spacing: 0
        Layout.leftMargin: slider.leftPadding + slider.handle.width * 0.2 ; Layout.rightMargin: slider.rightPadding + slider.handle.width * 0.2
        Layout.fillWidth: true

        Repeater {
            id: repeater
            model: control.model == null ? null
                                         : control.model.length === undefined ? control.model + 1
                                                                              : control.model
            Label {
                id: text
                Layout.fillWidth: true
                Layout.preferredWidth: index == 0 || index == repeater.count - 1 ? 1
                                                                                 : 2
                text: modelData
                wrapMode: Label.WordWrap
                elide: Qt.AlignRight
                horizontalAlignment: index == 0 ? Qt.AlignLeft
                                                : index == repeater.count - 1 ? Qt.AlignRight
                                                                              : Qt.AlignHCenter
                verticalAlignment: Qt.AlignTop
            }
        }
    }
}


