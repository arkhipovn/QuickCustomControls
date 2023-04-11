import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Controls.Material 2.9
import QCC.Controls 1.0 as QCC

QCC.TextField {
    id: control
    placeholderText: qsTr("Password")

    property bool checked: false
    echoMode: checked ? TextInput.Normal : TextInput.Password

    //    width: parent.width
    //    incorrectInput: control.text !== settings.connectionSettings.password

    rightAreaDelegate: QCC.SymbolButton {
        height: control.implicitHeight * 0.9; width: height
        anchors.verticalCenter: parent.verticalCenter

        checkable: true
        checked: control.checked
        onClicked: control.checked = checked

        symbol: checked ? "\uf06e" : "\uf070"
        color: checked ? Material.accent : Material.backgroundDimColor

        toolTip.text: checked ? qsTr("Hide password") : qsTr("Show password")
    }

    message: acceptableInput || text.length === 0 ? ""
                                                  : qsTr("Wrong password")
}
