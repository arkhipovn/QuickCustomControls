import QtQuick 2.12
import QtQuick.Controls 2.12
import QtCharts 2.3

Label {
    property var chartView: null
    visible: chartView !== null && chartView.marker.active

    //                font: chartView.legend.font
    font.pixelSize: chartView == null ? Qt.application.font.pixelSize
                                      : chartView.legend.font.pixelSize

    property int precision: 4

    property string numberFormat: 'g'

    text: {
        if(chartView == null || !chartView.marker.active)
            return ""

        var s = chartView.series(0)
        if(s === null) return ""

        var axis_x = chartView.axisX(chartView.series(0))
        var axis_y = chartView.axisY(chartView.series(0))

        if(chartView.marker.pressValue === chartView.marker.movedValue) {
            // Не сдвинулись с места. Вывести текущее значение
            return "x = " + (axis_x instanceof DateTimeAxis ? Qt.formatDateTime(new Date(chartView.marker.pressValue.x), axis_x.format)
                                                            : Number(chartView.marker.pressValue.x).toLocaleString(Qt.locale(), numberFormat/*axis_x.labelFormat*/, precision))
                    + "; y = " + (axis_y instanceof DateTimeAxis ? Qt.formatDateTime(new Date(chartView.marker.pressValue.y), axis_y.format)
                                                                 : Number(chartView.marker.pressValue.y).toLocaleString(Qt.locale(), numberFormat/*axis_y.labelFormat*/, precision))
        }
        else {
            // Вывести разницу
            var dx = Math.abs(chartView.marker.movedValue.x - chartView.marker.pressValue.x)
            var dy = Math.abs(chartView.marker.movedValue.y - chartView.marker.pressValue.y)

            return "|X2 - X1| = " + (axis_x instanceof DateTimeAxis ? Number(dx / 1000).toLocaleString(Qt.locale(), numberFormat, precision)
                                                                    : Number(dx).toLocaleString(Qt.locale(), numberFormat/*axis_x.labelFormat*/, precision)) +
                    "; |Y2 - Y1| = " + (axis_y instanceof DateTimeAxis ? Number(dy / 1000).toLocaleString(Qt.locale(), numberFormat, precision)
                                                                       : Number(dy).toLocaleString(Qt.locale(), numberFormat/*axis_y.labelFormat*/, precision))
        }
    }
}

