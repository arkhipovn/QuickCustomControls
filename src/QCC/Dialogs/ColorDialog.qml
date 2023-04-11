import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QCC.Controls 1.0 as QCC
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

ApplicationWindow {
    id: dialog
    visible: true
    width: 650; height: 400
//    flags: Qt.Dialog
    font.capitalization: Font.MixedCase
    modality: Qt.WindowModal

    signal accepted(var color)
    signal rejected()

    onClosing: {
        if(rowLayout.accepted) dialog.accepted(canvas.currentColor)
        else dialog.rejected()
    }

    RowLayout {
        id: rowLayout
        anchors { fill: parent; margins: 10; }
        spacing: 10

        property bool accepted: false

        QCC.ColorSelectionItem {
            id: canvas
            Layout.fillHeight: true; Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true;
            width: colorInfoColumn.width

            Pane {
                id: colorArea
                anchors { top: parent.top; bottom: colorInfoColumn.top; bottomMargin: rowLayout.spacing; }

                width: colorInfoColumn.width
                padding: 0
                background: Rectangle {
                    color: canvas.currentColor
                    layer.enabled: true
                    layer.effect: ElevationEffect {
                        elevation: 3
                    }
                }
            }

            Column {
                id: colorInfoColumn
                anchors.bottom: parent.bottom

                QCC.TextField {
                    width: parent.width
                    placeholderText: qsTr("Red")
                    text: 255 * canvas.currentColor.r
                    readOnly: true
                }

                QCC.TextField {
                    width: parent.width
                    placeholderText: qsTr("Green")
                    text: 255 * canvas.currentColor.g
                    readOnly: true
                }

                QCC.TextField {
                    width: parent.width
                    placeholderText: qsTr("Blue")
                    text: 255 * canvas.currentColor.b
                    readOnly: true
                }

                QCC.TextField {
                    width: parent.width
                    placeholderText: qsTr("HEX")
                    text: canvas.currentColor
                    readOnly: true
                }

                Row {
                    spacing: rowLayout.spacing

                    Button {
                        text: qsTr("OK")
                        onClicked: {
                            rowLayout.accepted = true
                            dialog.close()
                        }
                    }

                    Button {
                        text: qsTr("Cancel")
                        onClicked: {
                            rowLayout.accepted = false
                            dialog.close()
                        }
                    }
                }
            }
        }   // Item Info

    }   // RowLayout
}

