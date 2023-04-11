import QtQuick 2.0
import QCC.Fonts 1.0
import QCC.Controls 1.0
import QCC.DataView.ChartViewExtension 1.0

Row {
    id: toolBar

    property var chartView: null

    property color color: "blue"
    property color selectionColor: "gray"

    property alias squareBackground: homeButton.squareBackground
    property bool hoverEnabled: true

    property alias homeButtonVisible: homeButton.visible
    property alias markerButtonVisible: markerButton.visible
    property alias zoomButtonVisible: zoomButton.visible

    property alias topInset: homeButton.topInset
    property alias bottomInset: homeButton.bottomInset

    SymbolButton {
        id: homeButton
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        symbol: "\uf015"    // home
        symbolScale: 1.3
        opacity: toolBar.enabled ? 1.0 : 0.6

        color: toolBar.color
        toolTip.text: qsTr("Home")

        onClicked: {
            if(chartView == null) return
            chartView.resetTransform()
        }
    }

    SymbolButton {
        id: markerButton
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        opacity: toolBar.enabled ? 1.0 : 0.6

        topInset: homeButton.topInset
        bottomInset: homeButton.bottomInset
        squareBackground: homeButton.squareBackground

        symbol: "\uf041"    // map-marker
        toolTip.text: qsTr("Marker")
        symbolScale: 1.1

        color: chartView == null ? toolBar.color : (chartView.handlerMode ===  ChartViewExtension.Marker ? toolBar.selectionColor : toolBar.color)
        onClicked: {
            if(chartView == null) return
            chartView.handlerMode = ChartViewExtension.Marker
        }
    }

    SymbolButton {
        id: zoomButton
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        opacity: toolBar.enabled ? 1.0 : 0.6

        topInset: homeButton.topInset
        bottomInset: homeButton.bottomInset
        squareBackground: homeButton.squareBackground

        symbol: "\uf047"    // arrows
        //        symbolScale: 0.9

        toolTip.text: qsTr("Zoom")
        color: chartView == null ? toolBar.color : (chartView.handlerMode === ChartViewExtension.Zooming ? toolBar.selectionColor : toolBar.color)

        onClicked: {
            if(chartView == null) return
            chartView.handlerMode = ChartViewExtension.Zooming
        }
    }
}
