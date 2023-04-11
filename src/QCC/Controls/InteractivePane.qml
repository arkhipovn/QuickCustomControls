import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import QtQuick.Controls 2.12

Pane {
    id: control

    hoverEnabled: true

    property int radius: 2

    property bool clickable: true
    property bool rippleEnabled: true

    property bool optimized: false  // убирает тень по умолчанию..

    property int elevation: optimized ? (controlMouseArea.pressed && clickable ? 3
                                                                               : hovered ? 1 : 0)
                                      : (controlMouseArea.pressed && clickable ? 8
                                                                               : hovered ? 4 : 2)

    property color color: "white"
    property color activeColor: color//Material.rippleColor
    property color borderColor: "#E6E6E6"   // только в режиме optimized

    property alias pressed: controlMouseArea.pressed
    property alias pressAndHoldInterval: controlMouseArea.pressAndHoldInterval
    property alias acceptedButtons: controlMouseArea.acceptedButtons
    property alias pressedButtons: controlMouseArea.pressedButtons


    signal pressAndHold(var mouse)
    signal doubleClicked(var mouse)
    signal clicked(var mouse)
    signal entered()
    signal exited()
    signal released(var mouse)

    readonly property alias mouseArea: controlMouseArea

    background: Rectangle {
        radius: control.radius
        color: control.color

        layer.enabled: /*control.enabled && */control.elevation > 0
        layer.effect: ElevationEffect {
            elevation: control.elevation
        }

        border.width: optimized && !layer.enabled ? 1 : 0
        border.color: hovered ? "transparent" : control.borderColor

        //@N переделать..
        Ripple {
            id: ripple
            enabled: control.rippleEnabled
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            clipRadius: 2
            width: parent.width; height: parent.height
            pressed: controlMouseArea.pressed && control.clickable
            anchor: controlMouseArea
            active: (controlMouseArea.containsMouse || control.visualFocus) && enabled
            color: control.activeColor
        }

        //        Ripple {
        //            id: ripple
        //            enabled: control.rippleEnabled
        //            implicitWidth: parent.width; implicitHeight: parent.height
        //            x: (parent.width - width) / 2
        //            y: (parent.height - height) / 2

        ////                        clipRadius: control.radius
        //            width: parent.width - control.radius/* / 2*/; height: parent.height - control.radius /*/ 2*/
        //            pressed: controlMouseArea.pressed && control.clickable
        //            anchor: controlMouseArea
        //            active: (controlMouseArea.containsMouse || control.visualFocus) && enabled
        //            color: control.activeColor
        //        }

        MouseArea {
            id: controlMouseArea
            anchors.fill: parent
            enabled: clickable || hoverEnabled
            hoverEnabled: control.hoverEnabled

            onClicked: if(control.clickable) control.clicked(mouse)
            onPressAndHold: if(control.clickable) control.pressAndHold(mouse)
            onDoubleClicked: if(control.clickable) control.doubleClicked(mouse)
            onEntered: control.entered()
            onExited: control.exited()
            onReleased: if(control.clickable) control.released(mouse)
        }
    }
}
