import QtQuick 2.12

FocusScope {
    id: control
    activeFocusOnTab: false

    clip: true
    //    default property alias contentItem: body.data
    default property alias contentItem: loader.sourceComponent

    //    implicitWidth: loader.item ? loader.item.implicitWidth : 0
    //    implicitHeight: loader.item ? loader.item.implicitHeight : 0

    //    Loader {
    //        id: loader
    //    }

    Item {
        id: body
    }

}


/*
import QtQuick 2.12
Rectangle { border.width: 1
//Item {
    id: control
    //    clip: true

    default property alias contentItem: bodyItem.data

    property bool clickable: true
    property alias acceptedButtons: controlMouseArea.acceptedButtons
    property alias pressAndHoldInterval: controlMouseArea.pressAndHoldInterval

    property alias hoverEnabled: controlMouseArea.hoverEnabled

    property alias hovered: controlMouseArea.containsMouse
    property alias pressed: controlMouseArea.pressed

    property alias cursorShape: controlMouseArea.cursorShape

    signal clicked(var mouse)
    signal pressAndHold(var mouse)
    signal entered()
    signal exited()
    signal doubleClicked(var mouse)

    Item {
        id: bodyItem
        width: parent.width; height: parent.height

        // чтобы сигнал отправлялся после окончания анимаций
        onScaleChanged: if(control.state === "hold" && scale == 1.0) control.pressAndHold(controlMouseArea.holdMouse)

        Behavior on scale {
            NumberAnimation { duration: 200; }
        }
    }

    MouseArea {
        id: controlMouseArea
        width: parent.width; height: parent.height

        enabled: clickable || hoverEnabled
        property var holdMouse: null    // чтобы запомнить место зажатия
        //        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressAndHold: { // сигнал отправляется выше (onScaleChanged). После окончания анимаций всех..
            if(control.clickable) {
                controlMouseArea.holdMouse = mouse    // запоминаем где нажали
                control.state = "hold"
            }
        }
        onPressed: if(control.clickable) control.state = "pressed"
        onReleased: control.state = "rest"


        onClicked: if(control.clickable) control.clicked(mouse)
        onEntered: control.entered()
        onExited: control.exited()
        onDoubleClicked: if(control.clickable) control.doubleClicked(mouse)
    }


    state: "rest"
    states: [
        State {
            name: "pressed"

            PropertyChanges {
                target: bodyItem //control
                scale: 0.99
            }
        },
        State {
            name: "rest"

            PropertyChanges {
                target: bodyItem //control
                scale: 1.0
            }
        },
        State {
            name: "hold"
            PropertyChanges {
                target: bodyItem //control
                scale: 1.0
            }
        }
    ]   // states

    transitions: [
        Transition {
            from: "pressed"
            to: "hold"

            PropertyAnimation {
                properties: "scale"
                to: 0.98
                duration: 100
            }
        }
    ]   // transitions
}








*/
