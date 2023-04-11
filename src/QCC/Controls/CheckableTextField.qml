import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QCC.Controls 1.0 as QCC
import Theme 1.0

QCC.TextField {
    id: control
    horizontalAlignment: Qt.AlignLeft

    property bool checked: false
    readOnly: !checked

    property string toolTipText: ""

    leftAreaDelegate: QCC.SymbolButton {
        id: button
        height: control.implicitHeight * 0.9; width: height
        anchors.verticalCenter: parent.verticalCenter

        checkable: true
        checked: control.checked

        symbol: checked ? "\uf14a" : "\uf096"
        color: checked ? Material.accent : Material.backgroundDimColor

        onClicked: control.checked = checked

        toolTip.text: control.toolTipText
    }
}
