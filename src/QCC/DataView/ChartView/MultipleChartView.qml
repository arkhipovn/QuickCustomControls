import QtQuick 2.12
import QtQuick.Controls 2.12
import QCC.DataView.ChartView 1.0 as DV
import QCC.Controls 1.0 as QCC
import QtQuick.Controls.Material 2.12
//import QtQuick.Dialogs 1.0
import QCC.Dialogs 1.0 as QD
import QtQuick.Layouts 1.12

Pane {
    id: multipleChartView
    padding: 1

    property alias seriesHeaderSpacing: seriesTitleFlow.spacing

    property alias labelsColor: chartViewArea.labelsColor
    property alias labelsSize: chartViewArea.labelsSize
    property alias markerColor: chartViewArea.markerColor

    property alias chartView: chartViewArea
    property alias count: chartViewArea.count
    property alias axis: chartViewArea.axis

    property bool removable: false

    function setAxis(component) {
        var obj = component.createObject(chartViewArea)
        chartViewArea.axis = obj
    }

    //@N доделать..
    //    default property alias chartViewContent: chartViewArea.data

    property alias plotArea: chartViewArea.plotArea
    property alias dragHandler: chartViewArea.dragHandler
    property var funcCalcAverage: chartViewArea.funcCalcAverage

    function forceAxisRange(init) { // если true, то значит не смотрим на то, что visible = false
        chartViewArea.forceAxisRange(init)
    }

    property var source: null

    signal lineRemoved(var index)

    onSourceChanged: {
        // удалим все старое..
        if(chartViewArea.count > 0) {
            chartViewArea.removeSeries()
            chartViewArea.forceAxisRange()
        }

        if(source == null) return

        // есть что отрисовать..
        for(var i = 0; i < source.count; ++i) {
            var series = multipleChartView.getLineSeries(i)
            source.update(i, series)
        }
        chartViewArea.forceAxisRange(true)
    }

    Connections {
        target: source
        onSampleChanged: {
            if(index == -1) {
                for(var i = 0; i < multipleChartView.source.count; ++i) {
                    var series = multipleChartView.getLineSeries(i)
                    multipleChartView.source.update(i, series)
                }
                chartViewArea.forceAxisRange()
            }
            else {
                series = multipleChartView.getLineSeries(index)
                multipleChartView.source.update(index, series)
                chartViewArea.forceAxisRange()
            }
        }

        onSampleRemoved: {
            if(index == -1) {
                chartViewArea.removeSeries()
                chartViewArea.forceAxisRange()
            }
            else {
                chartViewArea.removeSeries(index)
                chartViewArea.forceAxisRange()
            }
        }
    }

    function getLineSeries(i) {
        if(chartViewArea.count > i) {// уже есть такой
            var lineseries = chartViewArea.getSeries(i)
            lineseries.label = multipleChartView.source.getTitle(i) // сверим имя..
        }
        else {  // нет, нужно создать
            var label = multipleChartView.source.getTitle(i)
            lineseries = chartViewArea.createSeries(DV.ChartView.LineSeries, label)
        }
        return lineseries
    }

    //    Component.onCompleted: {
    //        if(source === null) return;
    //        for(var i = 0; i < source.count; ++i) {
    //            var series = multipleChartView.getLineSeries(i)
    //            source.update(i, series)
    //        }
    //        chartViewArea.forceAxisRange(true)
    //    }

    // список линий
    Flow {
        id: seriesTitleFlow
        padding: multipleChartView.font.pixelSize
        width: parent.width
        spacing: multipleChartView.font.pixelSize * 0.4

        Repeater {
            model: chartViewArea.listSeries

            delegate: QCC.InteractivePane {
                padding: seriesTitleFlow.spacing
                activeColor: Material.rippleColor
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                //                hoverEnabled: multipleChartView.hoverEnabled

                property bool checked: true
                elevation: checked ? (pressed ? 8
                                              : hovered ? 4 : 2)
                                   : (pressed ? 4
                                              : hovered ? 2 : 0)

                contentItem: RowLayout {
                    spacing: seriesTitleFlow.spacing
                    implicitHeight: multipleChartView.font.pixelSize * 2

                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: multipleChartView.font.pixelSize * 0.8
                        Layout.preferredHeight: multipleChartView.font.pixelSize * 0.6
                        radius: 2
                        color: modelData.color
                    }

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.maximumWidth: multipleChartView.font.pixelSize * 12
                        elide: Qt.ElideLeft
                        text: label
                        //@N max len
                    }

                    Label {
                        id: removeButton
                        text: "×"
                        Layout.alignment: Qt.AlignVCenter
                        enabled: removable
                        visible: hovered && enabled
                        color: "red"
                        background: MouseArea {
                            onClicked: multipleChartView.lineRemoved(index)
                        }
                    }

                }

                onClicked: {
                    if(mouse.button & Qt.RightButton) {
                        // окно выбора цвета..
                        colorDialogLoader.active = true
                    }
                    else {
                        checked = !checked
                        var s = chartViewArea.getSeries(index)
                        if(s === null) return
                        if(checked) s.visible = true
                        else s.visible = false
                        s.enabled = s.visible
                        chartViewArea.forceAxisRange()
                    }
                }

                Component.onDestruction: colorDialogLoader.active = false

                Loader {
                    id: colorDialogLoader
                    active: false

                    sourceComponent: QD.ColorDialog {
                        title: qsTr("Please choose a color")
                        onAccepted: {
                            var s = chartViewArea.getSeries(index)
                            if(s === null) return
                            s.color = color
                            colorDialogLoader.active = false
                        }
                        onRejected: {
                            colorDialogLoader.active = false
                        }
                    }

                    /*
                    sourceComponent: ColorDialog {
                        visible: true
                        title: qsTr("Please choose a color")
                        onAccepted: {
                            var s = chartViewArea.getSeries(index)
                            if(s === null) return
                            s.color = color
                            colorDialogLoader.active = false
                        }
                        onRejected: {
                            colorDialogLoader.active = false
                        }
                    }
                    */
                }   // colorDialogLoader

            }
        }
    }

    // график
    DV.ChartViewArea {
        id: chartViewArea
        anchors { left: parent.left; right: parent.right; top: seriesTitleFlow.bottom; bottom: parent.bottom; }
        //        dragHandler.mode: DV.AxisDragHandler.Marker

        labelsSize: multipleChartView.font.pixelSize
        labelsColor: Material.foreground
        markerColor: Material.foreground

        onAxisChanged: {
            //@N а нужно ли отрисовывать линии?
            chartViewArea.forceAxisRange(true)
        }
    }
}

