import QtQuick 2.12
import QtQuick.Controls 2.12

Label {
    id: control
    topPadding: font.pixelSize * 0.5; bottomPadding: font.pixelSize * 0.5
    leftPadding: 1; rightPadding: 1
    font.underline: mouseArea.containsMouse

    property alias pressed: mouseArea.pressed
    property alias pressAndHoldInterval: mouseArea.pressAndHoldInterval
    property alias acceptedButtons: mouseArea.acceptedButtons
    property alias pressedButtons: mouseArea.pressedButtons

    signal pressAndHold(var mouse)
    signal doubleClicked(var mouse)
    signal clicked(var mouse)
    signal entered()
    signal exited()
    signal released(var mouse)

    background: MouseArea {
        id: mouseArea
        hoverEnabled: true
        onClicked: control.clicked(mouse)
        onPressAndHold: control.pressAndHold(mouse)
        onDoubleClicked: control.doubleClicked(mouse)
        onEntered: control.entered()
        onExited: control.exited()
        onReleased: control.released(mouse)
    }
}
