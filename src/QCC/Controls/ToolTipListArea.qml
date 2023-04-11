import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import QCC.Controls.Private 1.0

ListView {
    id: control
    spacing: Material.menuItemVerticalPadding
    z: 10
    clip: true
    boundsBehavior: Flickable.StopAtBounds
    interactive: false

    property int duration: 300
    property alias timeout: toolTipListModel.timeout
    property alias listType: toolTipListModel.listType

    function open(text, background, foreground) {
        toolTipListModel.append(text, background, foreground)
    }

    model: ToolTipListModel {
        id: toolTipListModel
    }

    add: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: control.duration; easing.type: Easing.InOutQuad; }
        //        NumberAnimation { properties: "x,y"; duration: control.duration; easing.type: Easing.InOutQuad; }
    }

    displaced: Transition {
        NumberAnimation { properties: "x,y"; duration: control.duration; easing.type: Easing.InOutQuad; }
        NumberAnimation { property: "opacity"; to: 1.0; duration: control.duration; easing.type: Easing.InOutQuad; }
    }

    delegate: Item {
        id: itemDelegate
        width: ListView.view.width
        height: label.height

        Label {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            padding: Material.menuItemVerticalPadding
            width: parent.width > implicitWidth ? implicitWidth // А точно так делать?
                                                : parent.width

            wrapMode: Label.WordWrap
            text: display
            color: foregroundColor

            background: Rectangle {
                radius: 4
                color: backgroundColor
                border.color: Material.backgroundDimColor

                MouseArea {
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    width: parent.width; height: parent.height
                    onClicked: toolTipListModel.removeAt(index)
                }
            }
        }
    }
}
