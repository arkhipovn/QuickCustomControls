import QtQuick 2.12
import QCC.Fonts 1.0
import QCC.Controls 1.0
import QCC.DataView.ChartView 1.0

Row {
    id: toolBar
    property var dragHandler: null

    property color color: "blue"
    property color selectionColor: "gray"
    property bool hoverEnabled: true

    property alias homeButtonVisible: homeButton.visible
    property alias markerButtonVisible: markerButton.visible
    property alias zoomButtonVisible: zoomButton.visible

    property alias inset: homeButton.topInset
    property alias squareBackground: homeButton.squareBackground

    SymbolButton {
        id: homeButton
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: toolBar.hoverEnabled
        symbol: "\uf015"    // home
        symbolScale: 1.2
        opacity: toolBar.enabled ? 1.0 : 0.6

        bottomInset: topInset

        color: toolBar.color
        toolTip.text: qsTr("Home")

        onClicked: {
            if(dragHandler == null) return
            dragHandler.resetTransform()
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
        symbolScale: 1.05
        color: dragHandler == null ? toolBar.color : (dragHandler.mode === AxisDragHandler.Marker ? toolBar.selectionColor : toolBar.color)

        onClicked: {
            if(dragHandler == null) return
            dragHandler.mode = AxisDragHandler.Marker
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
        color: dragHandler == null ? toolBar.color : (dragHandler.mode === AxisDragHandler.Zooming ? toolBar.selectionColor : toolBar.color)

        onClicked: {
            if(dragHandler == null) return
            dragHandler.mode = AxisDragHandler.Zooming
        }
    }
}

