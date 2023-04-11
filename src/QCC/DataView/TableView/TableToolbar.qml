import QtQuick 2.12
import QCC.Fonts 1.0
import QCC.Controls 1.0
import QtQuick.Controls 2.12 as Controls

Row {
    id: toolBar
    property var tableView: null
    property bool hoverEnabled: true
    property color color: "blue"

    property alias saveButtonActive: saveButtonLoader.active
    property alias printButtonActive: printButtonLoader.active

    property real popupMargins: 20

    Loader {
        id: saveButtonLoader
        active: true
        width: active ? implicitWidth : 0; height: active ? implicitHeight : 0

        sourceComponent: SymbolButton {
            symbol: "\uf0c7" // floppy-o
            color: toolBar.color
            hoverEnabled: toolBar.hoverEnabled

            onClicked: {
                if(tableView == null) return
                if(tableView.selectedIndexes.length > 0) saveMenu.open()    // откроем меню выбора
                else tableView.saveToCSV(true)  // сразу сохраним все..
            }
            Controls.Menu {
                id: saveMenu
                margins: toolBar.popupMargins
                Controls.MenuItem {
                    text: qsTr("Save selection");
                    onTriggered: tableView.saveToCSV(false)
                }
                Controls.MenuItem {
                    text: qsTr("Save the entire table");
                    onTriggered: tableView.saveToCSV(true)
                }
            }
        }
    }

    Loader {
        id: printButtonLoader
        active: true
        width: active ? implicitWidth : 0; height: active ? implicitHeight : 0

        sourceComponent: SymbolButton {
            symbol: "\uf02f" // floppy-o
            color: toolBar.color
            hoverEnabled: toolBar.hoverEnabled

            onClicked: {
                if(tableView == null) return
                if(tableView.selectedIndexes.length > 0) printMenu.open()    // откроем меню выбора
                else tableView.printTable(true)  // сразу сохраним все..
            }

            Controls.Menu {
                id: printMenu
                margins: toolBar.popupMargins

                Controls.MenuItem {
                    text: qsTr("Print selection");
                    onTriggered: tableView.printTable(false)
                }
                Controls.MenuItem {
                    text: qsTr("Print the entire table");
                    onTriggered: tableView.printTable(true)
                }
            }
        }
    }
}


//    Component {
//        id: dialogSaveFileComponent

//        FileDialog {
//            fileMode: FileDialog.SaveFile
//            nameFilters: [ "Image files (*.jpg *.png *bmp)", "All files (*)" ]

//            onAccepted: {
//                itemSave.save(file.toString().replace("file:///", ""))
//            }
//            onVisibleChanged: if(!visible) itemSave.close()
//        }
//    }





