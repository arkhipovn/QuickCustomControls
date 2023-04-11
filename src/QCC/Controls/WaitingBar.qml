import QtQuick 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
    id: waitingbar
    width: 100
    height: 5
    clip: true

    property alias progressColor: progressbar.color
    property alias duration: animation.duration

    property alias running: animation.running

    //    prop
    /*
    Row {
        id: progressbar
        spacing: 5
        height: parent.height

        Repeater {
            model: 5
            height: parent.height

            Rectangle {
                height: parent.height
                width: height
                radius: width * 0.5
                color: "blue"
            }
        }

        NumberAnimation on x {
            id: animation
            loops: Animation.Infinite
            running: waitingbar.opacity == 1.0 ? true : false
            from: -progressbar.width
            to: waitingbar.width
            duration: 1500
        }
    }
*/

    Rectangle {
        id: progressbar
        width: parent.width * 0.2
        height: parent.height
        color: Material.accentColor

        NumberAnimation on x {
            id: animation
            loops: Animation.Infinite
            running: waitingbar.visible
            from: -progressbar.width
            to: waitingbar.width
            duration: 1000
            easing.type: Easing.OutInSine   //OutInQuint
        }
    }

}
