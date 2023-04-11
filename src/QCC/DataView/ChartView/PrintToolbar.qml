import QtQuick 2.12
import QCC.Fonts 1.0
import QCC.Controls 1.0
import QCC.DataView.ChartView 1.0
//import Qt.labs.platform 1.1

Row {
    id: toolBar
    property var chart: null
    property var chartArea: null
    property bool hoverEnabled: true
    property color color: "blue"

    property alias inset: saveButton.topInset
    property alias squareBackground: saveButton.squareBackground

    SymbolButton {
        id: saveButton
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        opacity: toolBar.enabled ? 1.0 : 0.6

        bottomInset: topInset

        symbol: "\uf0c7" // floppy-o
//        symbolScale: 0.95
        toolTip.text: qsTr("Save to image")
        color: toolBar.color

        onClicked: {
            if(chart === null || chartArea === null) return
            chartArea.grabToImage(function(result) {
                chart.saveChart("", result.image)
            })
        }
    }

    SymbolButton {
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        opacity: toolBar.enabled ? 1.0 : 0.6

        topInset: saveButton.topInset
        bottomInset: saveButton.bottomInset
        squareBackground: saveButton.squareBackground

        symbol: "\uf02f"    // print
//        symbolScale: 0.95
        toolTip.text: qsTr("Print")
        color: toolBar.color

        onClicked: {
            if(chart === null || chartArea === null) return
            chartArea.grabToImage(function(result) {
                chart.printChart(result.image)
            })
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





