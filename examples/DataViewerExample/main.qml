import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.12

import QCC.DataView.ChartView 1.0
import QCC.Fonts 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("ChartViewExample")

    Connections {
        target: controller
        onDrawData: {
            chartView.dragHandler.resetTransform()
            lineSeries.samples = samples
            samples.destroy();
        }
    }

    Rectangle {
        id: background
        anchors { left: parent.left; right: parent.right; top: parent.top; bottom: buttonsRow.top; }

        ChartViewArea {
            id: chartView
            anchors.fill: parent


            ValueAxis {
                id: chartaxis
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


            /*
            DateTimeAxis {
                id: chartaxis

                //                factorActive: false

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
*/

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
