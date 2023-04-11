import QtQuick 2.12
import QtQuick.Controls 2.12
import QCC.DataView.ChartView 1.0

ChartView {
    id: chartview

    property alias dragHandler: axisDragHandler
    readonly property rect plotArea: axis == null ? Qt.rect(chartview.x, chartview.y, chartview.width, chartview.height) :
                                                    Qt.rect(chartview.x + axis.plotArea.x, chartview.y + axis.plotArea.y, axis.plotArea.width, axis.plotArea.height)    // подумать, что с этим делать.. chartview.y +

    property var funcCalcAverage: null

    property color labelsColor: "black"
    property int labelsSize: 10

    property alias markerColor: markeritem.markerColor

    function resetMarker() {
        labelAverage.visible = false
        markeritem.reset()
    }

    labelsFont: Qt.font({pixelSize: labelsSize})

    // сбросим все
    onAxisChanged: resetMarker()

    Connections {
        target: chartview.axis
        // изменились границы, обновим положение
        onAxisRangeChanged: markeritem.updateMarker()
    }

    // отдельный объект QObject, который отслеживает нажатия, зажатия и тд.. Он масштабирует ось. А от этого двигается график
    AxisDragHandler {
        id: axisDragHandler
        target: chartview.axis

        mode: AxisDragHandler.Zooming
        onModeChanged: {
            labelAverage.visible = false
            markeritem.reset()
        }

        onPressed: {
            labelAverage.visible = false
            markeritem.presspoint = chartview.mapToValue(point)
            markeritem.movepoint = markeritem.presspoint

            if(chartview.dragHandler.mode == AxisDragHandler.Marker)
                markeritem.visible = true
            else markeritem.visible = false
        }
        onClicked: {
            if(/*axisDragHandler.mode === AxisDragHandler.Marker && */button === Qt.RightButton) markeritem.reset() //@N проверить..
            else {
                markeritem.visible = true
                markeritem.presspoint = chartview.mapToValue(point)
                markeritem.movepoint = markeritem.presspoint
            }
        }
        onMove: {
            if(chartview.dragHandler.mode == AxisDragHandler.Marker || axisDragHandler.button === Qt.RightButton) {
                // добавим защиту. Если чуть-чуть сдвинулась рука при нажатии считать не будем
                if(Math.abs(markeritem.presspos.x - point.x) < 2 &&
                        Math.abs(markeritem.presspos.y - point.y) < 2) return
                markeritem.movepoint = chartview.mapToValue(point)
            }
        }
        onReleased: {
            if(funcCalcAverage == null) return
            if(chartview.dragHandler.mode == AxisDragHandler.Marker && markeritem.presspoint !== markeritem.movepoint) {
                labelAverage.visible = true
                labelAverage.average = funcCalcAverage(markeritem.presspoint.x, markeritem.movepoint.x)
            }
        }
    }

    // выделение
    Rectangle {
        id: highlighting
        z: 1

        color: Qt.rgba(chartview.markerColor.r, chartview.markerColor.g, chartview.markerColor.b, 0.3)

        opacity: 0.2

        visible: markeritem.presspoint !== markeritem.movepoint && (chartview.dragHandler.mode == AxisDragHandler.Zooming ? axisDragHandler.button === Qt.RightButton : true)

        x: markeritem.presspos.x; y: markeritem.presspos.y
        property real diffX: markeritem.movepos.x - x
        property real diffY: markeritem.movepos.y - y

        transform: Scale { origin { x: 0; y: 0 } xScale: highlighting.diffX > 0 ? 1 : -1; yScale: highlighting.diffY > 0 ? 1 : -1; }

        width: Math.abs(diffX)
        height: Math.abs(diffY)

// переделать на..
//        x: Math.min(pressPos.x, movedPos.x); y: Math.min(pressPos.y, movedPos.y)
//        property real diffX: marker.movedPos.x - marker.pressPos.x
//        property real diffY: marker.movedPos.y - marker.pressPos.y

//        width: diffX < 0 ? -diffX : diffX
//        height: diffY < 0 ? -diffY : diffY
    }


    Row {
        id: rowHeader
        height: axis == null ? 0 : axis.plotArea.y
        visible: axis != null
        anchors { right: parent.right; rightMargin: chartview.labelsSize * 0.2; }
        spacing: chartview.labelsSize

        Text {
            id: labelAverage
            visible: false
            anchors.verticalCenter: parent.verticalCenter

            property real average: 0

            text: "ȳ = " + Number(average).toLocaleString(Qt.locale(), 'g', 6)
            font.pixelSize: chartview.labelsSize
            color: chartview.labelsColor
        }

        Text {
            visible: markeritem.visible
            anchors.verticalCenter: parent.verticalCenter

            font.pixelSize: chartview.labelsSize
            color: chartview.labelsColor
            text: {
                if(markeritem.presspoint === markeritem.movepoint) return "y = " + Number(markeritem.presspoint.y).toLocaleString(Qt.locale(), 'g', 6)
                else {
                    // отображаем разность
                    //                    var temp = Math.abs(markeritem.movepoint.y - markeritem.presspoint.y)
                    //                    switch(chartView.axis.type) {
                    //                    case ChartView.LogValueAxisType:
                    //                        var dy = Math.pow(10, temp)
                    //                        break;
                    //                    case ChartView.ValueAxisType:
                    //                    case ChartView.DateTimeAxisType:
                    //                    default:
                    //                        dy = temp
                    //                        break;
                    //                    }
                    //                    print(temp, dy)
                    var dy = Math.abs(markeritem.movepoint.y - markeritem.presspoint.y)
                    return "|y2 - y1| = " + Number(dy).toLocaleString(Qt.locale(), 'g', 6)
                }
            }
        }

        Text {
            visible: markeritem.visible
            anchors.verticalCenter: parent.verticalCenter

            font.pixelSize: chartview.labelsSize
            color: chartview.labelsColor
            text: {
                if(chartview.axis == null) return ""
                if(markeritem.presspoint === markeritem.movepoint) {
                    if(chartview.axis.type === ChartView.DateTimeAxisType) return "t = " + chartview.axis.secsToString(markeritem.presspoint.x)
                    else "x = " + Number(markeritem.presspoint.x).toLocaleString(Qt.locale(), 'g', 6)
                }
                else {
                    // отображаем разность
                    var dx = Math.abs(markeritem.movepoint.x - markeritem.presspoint.x)
                    if(chartview.axis.type === ChartView.DateTimeAxisType) return "|t2 - t1| = " + (dx).toFixed(0) + " " + qsTr("s")
                    else "|t2 - t1| = " + Number(dx).toLocaleString(Qt.locale(), 'g', 6)
                }
            }
        }
    } //row labels

    Item {
        id: markeritem
        anchors.fill: parent
        visible: false
        opacity: 0.7
        z: 2
        property point presspoint: Qt.point(0, 0)
        property point presspos: chartview.mapToPosition(presspoint)

        property point movepoint: Qt.point(0, 0)
        property point movepos: chartview.mapToPosition(movepoint)

        function reset() {
            markeritem.visible = false;
            markeritem.presspoint = Qt.point(1, 1);
            markeritem.movepoint = Qt.point(1, 1);
        }

        property color markerColor: "blue"

        Rectangle {
            x: axis == null ? 0 : axis.plotArea.x; y: markeritem.presspos.y
            width: axis == null ? 0 : axis.plotArea.width; height: 2
            color: markeritem.markerColor
        }

        Rectangle {
            y: axis == null ? 0 : axis.plotArea.y; x: markeritem.presspos.x
            height: axis == null ? 0 : axis.plotArea.height; width: 2
            color: markeritem.markerColor
        }

        Rectangle {
            x: axis == null ? 0 : axis.plotArea.x; y: markeritem.movepos.y
            width: axis == null ? 0 : axis.plotArea.width; height: 2
            color: markeritem.markerColor
        }

        Rectangle {
            y: axis == null ? 0 : axis.plotArea.y; x: markeritem.movepos.x
            height: axis == null ? 0 : axis.plotArea.height; width: 2
            color: markeritem.markerColor
        }

        function updateMarker() {
            if(!markeritem.visible) return
            markeritem.presspos = Qt.binding(function() { return chartview.mapToPosition(markeritem.presspoint) })
            markeritem.movepos = Qt.binding(function() { return chartview.mapToPosition(markeritem.movepoint) })
        }

        onWidthChanged: markeritem.updateMarker()
        onHeightChanged: markeritem.updateMarker()
    }
}

