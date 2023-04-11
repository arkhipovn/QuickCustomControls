import QtQuick 2.12
import QCC.Validators 1.0
import QCC.Controls 1.0

TextField {
    id: dateInput
    implicitWidth: backgroundWidth
    horizontalAlignment: Qt.AlignHCenter

    signal dateChanged()

    property var dateTime: null
    text: dateTime == null ? ""
                           : Qt.formatDateTime(dateTime, format)

    property alias format: dateValidator.format
    property alias value: dateInput.text

    inputMask: dateValidator.inputMask;
    validator: DateValidator { id: dateValidator; }
    //    inputMethodHints: Qt.ImhDigitsOnly    //@N

    Keys.enabled: dateInput.enabled && !dateInput.readOnly
    Keys.onUpPressed: {
        var pos = cursorPosition
        text = dateValidator.up(text, pos)
        cursorPosition = pos
    }

    Keys.onDownPressed: {
        var pos = cursorPosition
        text = dateValidator.down(text, pos)
        cursorPosition = pos
    }

    Component.onCompleted: onTextChanged.connect(timer.restart)

    property alias dateChangeInterval: timer.interval

    function timerRestart() {
        timer.restart()
    }

    Timer {
        id: timer
        repeat: false; running: false
        interval: 500
        onTriggered: dateInput.dateChanged()
    }
}



