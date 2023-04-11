import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import QtQuick.Controls 2.12

Pane {
    id: control

    hoverEnabled: true
    property alias radius: background.radius
    property alias color: background.color

    property alias border: background.border

    property int elevation: controlMouseArea.enabled && controlMouseArea.pressed ? 8
                                                                                 : hovered ? 5 : 3

    property alias clickable: controlMouseArea.enabled
    readonly property alias mouseArea: controlMouseArea

    property alias pressed: controlMouseArea.pressed
    property alias pressAndHoldInterval: controlMouseArea.pressAndHoldInterval
    property alias acceptedButtons: controlMouseArea.acceptedButtons
    property alias pressedButtons: controlMouseArea.pressedButtons

    signal pressAndHold(var mouse)
    signal doubleClicked(var mouse)
    signal clicked(var mouse)
    signal released(var mouse)

    property alias ripple: ripple

    background: Rectangle {
        id: background
        radius: 4
        border { width: 0; color: Material.backgroundDimColor; }

        color: Material.backgroundColor

        layer.enabled: control.elevation > 0
        layer.effect: ElevationEffect {
            elevation: control.elevation
        }

        //@N переделать..
        Ripple {
            id: ripple
            enabled: false
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            clipRadius: 2
            width: parent.width; height: parent.height
            pressed: controlMouseArea.pressed && control.clickable
            anchor: controlMouseArea
            active: (controlMouseArea.containsMouse || control.visualFocus) && enabled
            color: Material.rippleColor
        }

        MouseArea {
            id: controlMouseArea
            anchors.fill: parent
            hoverEnabled: false
            enabled: false
            propagateComposedEvents: true
            onClicked: control.clicked(mouse)
            onPressAndHold: control.pressAndHold(mouse)
            onDoubleClicked: control.doubleClicked(mouse)
            onReleased: control.released(mouse)
        }
    }


}
