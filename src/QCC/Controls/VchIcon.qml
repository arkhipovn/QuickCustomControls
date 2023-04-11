import QtQuick 2.0

Image {
    id: control
    width: 150; height: width

    source: "qrc:/images/vchicon.png"
    fillMode: Image.PreserveAspectFit

    property var dateTime: null
    property alias shorthandColor: shorthand.color
    property color longhandColor: longhand.color
    property color secondhandColor: secondhand.color

    property alias animation: animationTimer
    property bool currentDateTime: false

    property int timeSpec: Qt.UTC
    // OffsetFromUTC
    //16200
    property int offsetFromUtc: 0

    Timer {
        id: animationTimer
        running: false; repeat: true
        triggeredOnStart: true
        interval: 100
        onTriggered: {
            if(control.dateTime == null) control.dateTime = new Date()
            if(!control.currentDateTime) {
                control.dateTime.setHours(control.dateTime.getHours() + 1)
                control.dateTime.setSeconds(control.dateTime.getSeconds() + 100);
            }
            else control.dateTime = new Date()
            dateTimeChanged()
        }
    }

    Rectangle {
        id: shorthand

        x: parent.width / 2 - width / 2; y: parent.height * 0.474 - height
        width: parent.paintedWidth * 0.035; height: parent.paintedHeight * 0.27

        smooth: true
        antialiasing: true
        color: "#b38080"
        border.color: "#997a7a"

        transform: Rotation {
            origin.x: shorthand.width * 0.5
            origin.y: shorthand.height *  0.83
            angle: dateTime == null ? 0
                                    : timeSpec == Qt.UTC ? 30 * dateTime.getUTCHours()
                                                         : timeSpec == Qt.LocalTime ? 30 * dateTime.getHours()
                                                                                    : 30 * (dateTime.getUTCHours() + Math.floor(offsetFromUtc / 3600))
        }
    }

    Rectangle {
        id: longhand

        x: parent.width / 2 - width / 2; y: parent.height * 0.489 - height
        width: parent.paintedWidth * 0.02; height: parent.paintedHeight * 0.35

        smooth: true
        antialiasing: true
        color: "#b38080"
        border.color: "#997a7a"

        function getAngle() {
            if(dateTime == null)
                return 01620

            switch(timeSpec) {
            case Qt.UTC:
                return dateTime.getUTCMinutes() * 6 + 3 * ((dateTime.getUTCSeconds() < 30) ? 0 : 1)
            case Qt.LocalTime:
                return dateTime.getMinutes() * 6 + 3 * ((dateTime.getSeconds() < 30) ? 0 : 1)
            case Qt.OffsetFromUTC:
                return (dateTime.getUTCMinutes() + Math.floor((offsetFromUtc % 3600) / 60)) * 6
                        + 3 * (((dateTime.getUTCSeconds() + offsetFromUtc % 3600 % 60) < 30) ? 0 : 1)
            default:
                return 0
            }
        }

        transform: Rotation {
            origin.x: longhand.width * 0.5
            origin.y: longhand.height *  0.83
            angle: longhand.getAngle()
        }
    }

    Rectangle {
        id: secondhand

        width: parent.paintedWidth * 0.01; height: parent.paintedHeight * 0.4
        x: parent.width / 2 - width / 2; y: parent.height * 0.498 - height

        smooth: true
        antialiasing: true
        color: "#8a7171" //"#9f9f9f" //"#c83325"
        border.color: "#997a7a"

        function getAngle() {
            if(dateTime == null)
                return 0

            switch(timeSpec) {
            case Qt.UTC:
                return 6 * dateTime.getUTCSeconds()
            case Qt.LocalTime:
                return 6 * dateTime.getSeconds()
            case Qt.OffsetFromUTC:
                return 6 * (dateTime.getUTCSeconds() + offsetFromUtc % 3600 % 60)
            default:
                return 0
            }
        }


        transform: Rotation {
            origin.x: secondhand.width * 0.5
            origin.y: secondhand.height *  0.83
            angle: secondhand.getAngle()
        }

        Rectangle {
            width: parent.width * 3; height: width; radius: width / 2
            x: secondhand.width * 0.5 - radius; y: secondhand.height * 0.25
            border { width: 1; color: "#c83325"; }
            color: "yellow"
        }
    }

}

