import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.12

import QCC.Controls 1.0
import QCC.DataView.ChartView 1.0
import QCC.Fonts 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("ChartViewExample")

    Rectangle {
        id: background
        anchors { left: parent.left; right: parent.right; top: parent.top; bottom: buttonsRow.top; }

        ChartViewArea {
            id: chartView
            anchors.fill: parent

            DateTimeAxis {
                id: chartaxis

                formatY: "dd.MM.yyyy hh:mm"
                timeSpec: Qt.UTC
                gridColor: "#DEDEDE"
                axisColor: "#C2C2C2"

                minX: lineSeries.minX; maxX: lineSeries.maxX
                minY: lineSeries.minY; maxY: lineSeries.maxY

                // можно менять, количество делений на сетке в зависимости от размера
                tickCountX: {
                    if(width > height) return 2
                    else return 1
                }
            }

            funcCalcAverage: function func(a, b) {
                return (a + b) / 2
            }

            LineSeries {
                id: lineSeries
                lineWidth: 2
                color: "blue"
            }   // LineSeries

        }   // ChartViewArea
    }


    Row {
        anchors.bottom: parent.bottom
        spacing: 10

        GraphToolbar {
            dragHandler: chartView.dragHandler
        }

        PrintToolbar {
            chartArea: background
            chart: chartView
        }
    }


    Row {
        id: buttonsRow
        anchors { bottom: parent.bottom; right: parent.right; }
        spacing: 10

        Button {
            text: "Draw big data"
            onClicked: controller.drawBigData()
        }

        Button {
            text: "Draw small data"
            onClicked: controller.drawSmallData()
        }
    }
}




//import QtQuick 2.12
//import QtQuick.Window 2.12
//import QtQuick.Controls 2.5

//import CChartView 1.0

//Window {
//    id: window
//    visible: true
//    width: 640
//    height: 480
//    title: qsTr("Hello World")

//    property var datasource: controller.getSource()

//    property real sizeText: Qt.application.font.pixelSize
//    property real defaultIndent: Qt.application.font.pixelSize * 2


//    function calcRange() {
//        switch(chartview.count) {
//        case 0:
//            var minX = 0; var maxX = 1;
//            var minY = 0; var maxY = 1;
//            break;
//        case 1:
//            var s = chartview.getSeries(0)
//            minX = s.minX; maxX = s.maxX;
//            minY = s.minY; maxY = s.maxY;
//            break;
//        default:
//            minX = Number.MAX_VALUE; maxX = Number.MIN_VALUE;
//            minY = Number.MAX_VALUE; maxY = Number.MIN_VALUE;

//            for(var i = 0; i < chartview.count; ++i) {
//                s = chartview.getSeries(i)
//                if(!s.visible) continue

//                if(minX > s.minX) minX = s.minX
//                if(maxX < s.maxX) maxX = s.maxX

//                if(minY > s.minY) minY = s.minY
//                if(maxY < s.maxY) maxY = s.maxY
//            }
//            break;
//        }

//        chartview.axis.minX = minX; chartview.axis.maxX = maxX;
//        chartview.axis.minY = minY; chartview.axis.maxY = maxY;
//    }


//    ListView {
//        id: listGraph
//        visible: chartview.count > 0
//        anchors { right: parent.right; top: parent.top; bottom: parent.bottom; margins: defaultIndent * 0.2; }
//        width: visible ? parent.width * 0.15 : 0

//        model: chartview.listSeries

//        spacing: defaultIndent * 0.2
//        property int lastIndex: -1

//        delegate: Rectangle {
//            color: "transparent"
//            border.color: "#E6E6E6"; border.width: checked ? 1 : 0
//            radius: 2

//            width: listGraph.width; height: sizeText * 2
//            scale: mouseArea.pressed ? 0.97 : 1.0
//            property bool checked: true

//            Row {
//                leftPadding: defaultIndent * 0.2
//                height: parent.height
//                spacing: defaultIndent * 0.2

//                Rectangle {
//                    anchors.verticalCenter: parent.verticalCenter
//                    width: listGraph.width * 0.15
//                    height: 3
//                    color: modelData.color
//                }

//                Text {
//                    anchors.verticalCenter: parent.verticalCenter
//                    text: label
//                    font.pixelSize: sizeText
//                    color: "#303030"
//                }
//            }

//            MouseArea {
//                id: mouseArea
//                anchors.fill: parent
//                onClicked: {
//                    checked = !checked
//                    var s = chartview.getSeries(index)
//                    if(checked) s.visible = true
//                    else s.visible = false
//                    window.calcRange()
//                }
//            }
//        }
//    } // listGraph


//    ChartView {
//        id: chartview
//        anchors { left: parent.left; right: listGraph.left; top: parent.top; bottom: rowbuttons.top; }

//        Component.onDestruction: {
//            if(window.datasource === null) return;
//            window.datasource.removeAll()
//        }

//        labelsSize: window.sizeText

//        function createLineSeries(i) {
//            if(chartview.count > i)
//                var lineseries = chartview.getSeries(i)
//            else {
//                var label = window.datasource.getTitle(i)
//                lineseries = chartview.createSeries(CChartView.LineSeries, label)
//            }
//            return lineseries
//        }

//        CDateTimeAxis {
//            id: chartaxis

//            formatY: "dd.MM.yyyy hh:mm"
//            timeSpec: Qt.UTC
//            gridColor: "#DEDEDE"
//            axisColor: "#C2C2C2"

//            // можно менять, количество делений на сетке в зависимости от размера
//            tickCountX: {
//                if(width > height) return 2
//                else return 1
//            }
//        }


//        Connections {
//            target: window.datasource

//            onRemovedData: {
//                chartview.removeSeries(i)
//                window.calcRange()
//            }
//            onDataChanged: {
//                var series = chartview.createLineSeries(i)
//                window.datasource.update(i, series)
//                window.calcRange()
//            }
//            onRemoveAllData: {
//                chartview.removeSeries()
//                window.calcRange()
//            }
//            onAveragingIntervalChanged: {
//                for(var i = 0; i < chartview.count; ++i) {
//                    var series = chartview.getSeries(i)
//                    window.datasource.update(i, series)
//                }
//                window.calcRange()
//            }
//        }
//    }

//    Row {
//        id: rowbuttons
//        anchors { bottom: parent.bottom; right: parent.right; }
//        height: parent.height * 0.1

//        Button {
//            text: "append"
//            onClicked: controller.drawSmallData()
//        }

//        Button {
//            text: "Remove first";
//            onClicked: datasource.remove(0)
//        }

//        //        Button {
//        //            text: "Set value";
//        //            onClicked: axisDragHandler.setValues(axis.minX, axis.maxX, 0, 0.05)
//        //        }

//        Button {
//            text: "Change decimation";
//            onClicked: window.datasource.decimation = 10
//        }
//    }
//}
