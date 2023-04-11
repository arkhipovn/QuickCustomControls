import QtQuick 2.12
import QtQuick.Controls 2.5
import QtCharts 2.3
import QCC.DataView.ChartViewExtension 1.0

//@N сейчас все завязано на выборе серии, а не оси. Переделать бы..

ChartView {
    id: chartView
    antialiasing: true

    property alias axisXMouseArea: axisXMouse
    property alias axisYMouseArea: axisYMouse

    property int handlerMode: ChartViewExtension.Marker        // Zooming/Marker/Inaction
    onHandlerModeChanged: resetMarker()

    signal resetTransform()  //@N пока не придумал как вернуть предыдущий масштаб.. Пока что по сигналу нужно вернуть назад все..
    //QtObject

    property Component chartLegend: null
    legend.visible: chartLegend == null

    margins {
        top: legend.visible ? 20
                            : legentLoader.height + legentLoader.y;
        bottom: legend.visible ? 20
                               : legentLoader.height + 10;      // font.pixelSize * 2
        left: legend.visible ? 20
                             : 0;
        right: legend.visible ? 20
                              : 0
    }

    Loader {
        id: legentLoader
        active: !legend.visible
        x: 20   //font.pixelSize * 2
        y: 20   //font.pixelSize * 2
        z: 10
        width: parent.width - x * 2
        sourceComponent: chartLegend
        onLoaded: {
            item.chartView = chartView
        }
    }

    property var marker: QtObject {
        //        id: markerSettings
        property real width: 2
        property color color: "blue"

        readonly property bool active: handlerMode !== ChartViewExtension.Marker ? false
                                                                                 : markerLoader.active
        property alias pressValue: chartSettings.pressValue
        property alias movedValue: chartSettings.movedValue
    }

    function resetMarker() {
        markerLoader.active = false
        chartSettings.reset()
    }

    function axisValue(value, toDate) {
        // Конвертирует в нужный формат
        return toDate ? new Date(value)
                      : value
    }

    MouseArea {
        id: mouseArea

        x: handlerMode === ChartViewExtension.Zooming ? 0
                                                      : plotArea.x;

        y: handlerMode === ChartViewExtension.Zooming ? 0
                                                      : plotArea.y;

        width: handlerMode === ChartViewExtension.Zooming ? parent.width
                                                          : plotArea.width;

        height: handlerMode === ChartViewExtension.Zooming ? parent.height
                                                           : plotArea.height;


        enabled: handlerMode !== ChartViewExtension.Inaction

        hoverEnabled: pressed
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        cursorShape: handlerMode === ChartViewExtension.Marker ? Qt.CrossCursor
                                                               : handlerMode === ChartViewExtension.Zooming && mouseArea.pressedButtons & Qt.LeftButton ? Qt.ClosedHandCursor
                                                                                                                                                        : Qt.ArrowCursor

        function chartViewPosition(mouse) {
            return Qt.point(mouse.x + x, mouse.y + y)
        }

        onPressed: {
            chartSettings.setPressValue(chartViewPosition(mouse))
            markerLoader.active = true
        }

        onClicked: {
            if(!chartSettings.isEqualValue()) return    // обрабатывать нажатие будет только в том случае, если курсор не двигался после зажатия

            if(mouse.button & Qt.RightButton) {
                // сбрасываем все

                if(handlerMode === ChartViewExtension.Zooming)
                    chartView.resetTransform()

                chartSettings.reset()
                markerLoader.active = false
            }
        }

        onPositionChanged: {
            // добавим защиту. Если чуть-чуть сдвинулась рука при нажатии считать не будем
            if(Math.abs(chartSettings.pressPos.x - x - mouseX) < 2 &&
                    Math.abs(chartSettings.pressPos.y - y - mouseY) < 2) {
                chartSettings.clearMovedValue()
                return
            }

            chartSettings.setMovedValue(chartViewPosition(mouse))

            if(handlerMode === ChartViewExtension.Zooming && pressedButtons & Qt.LeftButton)
                chartSettings.move()
        }

        onReleased: {
            if(handlerMode !== ChartViewExtension.Zooming)
                return
            if(mouse.button & Qt.RightButton)
                chartSettings.zoom()
        }

        //        onWheel: {
        //            if(handlerMode !== ChartViewExtension.Zooming)
        //                return
        //        }
    }

    Connections {
        target: chartView
        onSeriesAdded: {
            if(chartSettings.currentSeries === null) chartSettings.currentSeries = series
        }

        onSeriesRemoved: {
            if(count == 0) {
                // Если больше нет, то удаляем
                chartSettings.currentSeries = null
                return
            }

            if(chartSettings.currentSeries === series) chartSettings.currentSeries = chartView.series(0)
        }
    }

    QtObject {
        id: chartSettings

        property var currentSeries: null

        Component.onCompleted: {
            if(chartView.count > 0)
                currentSeries =  chartView.series(0)
        }

        onCurrentSeriesChanged: {
            chartView.resetMarker()
        }

        property point pressValue: Qt.point(1, 1)
        property point pressPos: currentSeries === null ? pressValue
                                                        : chartView.mapToPosition(pressValue, currentSeries)

        property point movedValue: Qt.point(1, 1)
        property point movedPos: currentSeries == null ? movedValue
                                                       : chartView.mapToPosition(movedValue, currentSeries)

        function setPressValue(pos) {
            pressValue = valueFromPosition(pos)
            movedValue = pressValue
        }

        function setMovedValue(pos) {
            movedValue = valueFromPosition(pos)
        }

        function clearMovedValue() {
            if(isEqualValue()) return
            movedValue = pressValue
        }

        function isEqualValue() {
            return pressValue === movedValue
        }

        function valueFromPosition(pos) {
            return chartSettings.currentSeries === null ? Qt.point(1, 1)
                                                        : chartView.mapToValue(pos, chartSettings.currentSeries)
        }

        function updatePosition() {
            pressPos = Qt.binding(function() { return currentSeries === null ? pressValue
                                                                             : chartView.mapToPosition(pressValue, currentSeries) })
            movedPos = Qt.binding(function() { return currentSeries === null ? movedValue
                                                                             : chartView.mapToPosition(movedValue, currentSeries) })
        }

        function reset() {
            pressValue = Qt.point(1, 1)
            movedValue = Qt.point(1, 1)
        }

        function zoom() {
            if(currentSeries === null) return
            if(isEqualValue()) return

            var axis_x = axisX(currentSeries)
            var axis_y = axisY(currentSeries)

            if(pressValue.x < movedValue.x) {
                axis_x.min = axisValue(pressValue.x, axis_x.format !== undefined)   // Не знаю как правильно сделать.. Почему нельзя получить тип...
                axis_x.max = axisValue(movedValue.x, axis_x.format !== undefined)
            }
            else {
                axis_x.min = axisValue(movedValue.x, axis_x.format !== undefined)
                axis_x.max = axisValue(pressValue.x, axis_x.format !== undefined)
            }

            if(pressValue.y < movedValue.y) {
                axis_y.min = axisValue(pressValue.y, axis_y.format !== undefined)
                axis_y.max = axisValue(movedValue.y, axis_y.format !== undefined)
            }
            else {
                axis_y.min = axisValue(movedValue.y, axis_y.format !== undefined)
                axis_y.max = axisValue(pressValue.y, axis_y.format !== undefined)
            }
        }

        function move() {
            var dx = pressValue.x - movedValue.x
            var dy = pressValue.y - movedValue.y

            var axis_x = axisX(currentSeries)
            var axis_y = axisY(currentSeries)

            if(axis_x.format === undefined) {   // Не знаю как правильно сделать.. Почему нельзя получить тип...
                axis_x.min += dx
                axis_x.max += dx
            }
            else {
                axis_x.min = new Date(axis_x.min.getTime() + dx);
                axis_x.max = new Date(axis_x.max.getTime() + dx);
            }

            if(axis_y.format === undefined) {
                axis_y.min += dy
                axis_y.max += dy
            }
            else {
                axis_y.min = new Date(axis_y.min.getTime() + dy);
                axis_y.max = new Date(axis_y.max.getTime() + dy);
            }
        }

    }

    Loader {
        id: markerLoader
        width: parent.width; height: parent.height
        active: false

        sourceComponent: handlerMode === ChartViewExtension.Marker ? markerRecComponent
                                                                   : handlerMode === ChartViewExtension.Zooming ? (mouseArea.pressedButtons & Qt.RightButton ? zoomingRecComponent
                                                                                                                                                             : null)
                                                                                                                : null
        Component {
            id: zoomingRecComponent

            Item {
                onWidthChanged: chartSettings.updatePosition()
                onHeightChanged: chartSettings.updatePosition()

                Rectangle {
                    color: marker.color
                    opacity: 0.15

                    x: Math.min(chartSettings.pressPos.x, chartSettings.movedPos.x); y: Math.min(chartSettings.pressPos.y, chartSettings.movedPos.y)
                    readonly property real diffX: chartSettings.movedPos.x - chartSettings.pressPos.x
                    readonly property real diffY: chartSettings.movedPos.y - chartSettings.pressPos.y

                    width: diffX < 0 ? -diffX : diffX
                    height: diffY < 0 ? -diffY : diffY
                }
            }
        }   // Component

        Component {
            id: markerRecComponent

            Item {
                onWidthChanged: chartSettings.updatePosition()
                onHeightChanged: chartSettings.updatePosition()

                Rectangle {
                    x: plotArea.x; y: chartSettings.pressPos.y - marker.width * 0.5
                    width: plotArea.width; height: marker.width
                    color: marker.color
                }

                Rectangle {
                    x: chartSettings.pressPos.x - marker.width * 0.5; y: plotArea.y
                    height: plotArea.height; width: marker.width
                    color: marker.color
                }

                Rectangle {
                    x: plotArea.x; y: chartSettings.movedPos.y - marker.width * 0.5
                    width: plotArea.width; height: marker.width
                    color: marker.color
                }

                Rectangle {
                    x: chartSettings.movedPos.x - marker.width * 0.5; y: plotArea.y
                    height: plotArea.height; width: marker.width
                    color: marker.color
                }
            }
        }   // Component
    }       // Marker Loader


    MouseArea {
        id: axisXMouse
        z: 2
        x: plotArea.x; y: plotArea.y + plotArea.height
        width: plotArea.width; height: chartView.height - y - margins.bottom

        //        Rectangle {
        //            width: parent.width; height: parent.height
        //            color: "brown"
        //            opacity: 0.1
        //        }
    }


    MouseArea {
        id: axisYMouse
        z: 2
        x: margins.left
        y: plotArea.y
        width: plotArea.x - x; height: plotArea.height

        //        FontMetrics {
        //            id: metrics
        //            //            font: label.font
        //        }

        //        onClicked: {
        //            //            x, width
        //            var w = width
        //            for(var i = 0; i < chartView.count; ++i) {
        //                var axis = axisY(series(i))

        //                print(i, (axis.max - axis.min), Number(axis.min).toString(), Number(axis.min).toLocaleString(Qt.locale(), axis.labelFormat))
        //                continue;

        //                var strMin = axis.format !== undefined ? Qt.formatDateTime(axis.min, axis.format)
        //                                                       : Number(axis.min).toLocaleString(Qt.locale(), axis.labelFormat)

        //                var strMax = axis.format !== undefined ? Qt.formatDateTime(axis.max, axis.format)
        //                                                       : Number(axis.max).toLocaleString(Qt.locale(), axis.labelFormat)

        //                print(i, strMin, strMax)
        //                continue;

        //                var axisWidth = Math.max(metrics.boundingRect(strMax).width, metrics.boundingRect(strMin).width)
        //                print(i, w, mouseX, axisWidth)
        //                if(mouseX < w && mouseX > w - axisWidth) {
        //                    print("finded", i)
        //                    axisRec.x = w - axisWidth
        //                    axisRec.width = axisWidth
        //                    return
        //                }

        //                w -= axisWidth
        //                print("not", i)
        //            }
        //        }


        //        Rectangle {
        //            anchors.fill: parent
        //            opacity: 0.1
        //            color: "red"
        //        }

        //        Rectangle {
        //            id: axisRec
        //            height: parent.height
        //            opacity: 0.1
        //            color: "red"
        //        }

    }


}











