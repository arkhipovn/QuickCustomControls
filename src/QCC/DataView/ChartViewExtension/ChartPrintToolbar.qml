import QtQuick 2.0
import QCC.Fonts 1.0
import QCC.Controls 1.0
import QCC.DataView.ChartViewExtension 1.0
import QCC.Utils 1.0 as QCC

Row {
    id: toolBar

    property var chartView: null

    property color color: "blue"

    property alias squareBackground: saveButton.squareBackground
    property bool hoverEnabled: true

    property alias topInset: saveButton.topInset
    property alias bottomInset: saveButton.bottomInset

    //    QCC.Utils {
    //        id: saver
    //    }

    SymbolButton {
        id: saveButton
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        opacity: toolBar.enabled ? 1.0 : 0.6

        symbol: "\uf0c7" // floppy-o
        //        symbolScale: 0.95
        toolTip.text: qsTr("Save to image")
        color: toolBar.color

        onClicked: {
            if(chartView === null) return
            chartView.grabToImage(function(result) {
                QCC.Utils.saveChart("", result.image)
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

        toolTip.text: qsTr("Print")
        color: toolBar.color

        onClicked: {
            if(chartView === null) return
            chartView.grabToImage(function(result) {
                QCC.Utils.printChart(result.image)
            })
        }
    }
}
