import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: legendItem

    z: 10
    height: flow.height
    property var chartView: null

    property bool hoverEnabled: true
    property bool removable: false

    signal clicked(int index)
    signal doubleClicked(int index)
    signal removeClicked(int index)

    property alias spacing: flow.spacing

    Connections {
        target: chartView
        onSeriesAdded: repeater.model = chartView.count
        onSeriesRemoved: repeater.model = chartView.count
    }

    onChartViewChanged: {
        if(chartView == null) {
            repeater.model = null
        }
        else {
            repeater.model = chartView.count
        }
    }

    Flow {
        id: flow

        width: getMaxRowWidth()
        anchors.horizontalCenter: parent.horizontalCenter

        function getMaxRowWidth() {
            var maxLegendWidth = legendItem.width

            var maxRowWidth = 0;
            var rowWidth = 0
            for(var i = 0; i < repeater.count; ++i) {
                var w = repeater.itemAt(i).width + flow.spacing
                if(rowWidth === 0) rowWidth = w
                else if(rowWidth + w >= maxLegendWidth) {
                    maxRowWidth = Math.max(maxRowWidth, rowWidth)
                    rowWidth = w
                }
                else rowWidth += w
            }

            return Math.max(maxRowWidth, rowWidth)
        }

        Repeater {
            id: repeater

            ItemDelegate {
                id: item
                implicitHeight: implicitContentHeight + topPadding + bottomPadding
                text: item.series.name

                readonly property var series: chartView.series(index)

                highlighted: !series.visible

                font.pixelSize: chartView.legend.font.pixelSize
                topInset: 0; bottomInset: 0

                topPadding: font.pixelSize * 0.3; bottomPadding: font.pixelSize * 0.3
                leftPadding: font.pixelSize * 0.5; rightPadding: font.pixelSize * 0.5

                hoverEnabled: legendItem.hoverEnabled

                onClicked: {
                    series.visible = highlighted
                    legendItem.clicked(index)
                }

                onDoubleClicked: legendItem.doubleClicked(index)

                contentItem: RowLayout {
                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: item.font.pixelSize * 0.6
                        Layout.preferredHeight: item.font.pixelSize * 0.6
                        //                        radius: 2
                        color: item.series.color
                        border.width: 1
                    }

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.maximumWidth: font.pixelSize * 12
                        elide: Qt.ElideMiddle
                        text: item.text
                    }

                    Label {
                        id: removeButton
                        text: "×"
                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillHeight: true
                        enabled: removable
                        visible: item.hovered && enabled
                        color: "red"
                        background: MouseArea {
                            onClicked: legendItem.removeClicked(index)
                        }
                    }
                }

                ToolTip {
                    visible: item.hovered
                    text: item.series.name
                    delay: 300
                }
            }
        }
    }



}





/*
RowLayout {
    id: legendFlow
    property var chartView: null

    property bool hoverEnabled: true
    property bool removable: false

    signal clicked(int index)
    signal doubleClicked(int index)
    signal removeClicked(int index)

    Connections {
        target: chartView
        onSeriesAdded: repeater.model = chartView.count
        onSeriesRemoved: repeater.model = chartView.count
    }
    onWidthChanged: print("LEGEND WIND CHANGED", width)

    Flow {
        id: flow
        spacing: 10

        function getMaxRowWidth() {
            var maxx = legendFlow.width
            print("getMaxRowWidth", legendFlow.width, legendFlow.implicitWidth)
            var maxWidth = 0;
            var rowWidth = 0
            for(var i = 0; i < repeater.count; ++i) {
                var w = repeater.itemAt(i).width + flow.spacing
                if(rowWidth === 0) rowWidth = w
                else if(rowWidth + w >= maxx) {
                    maxWidth = Math.max(maxWidth, rowWidth)
                    rowWidth = w
                }
                else rowWidth += w
                print(i, w, rowWidth, maxWidth)
            }

            return Math.max(maxWidth, rowWidth)
        }


        Layout.preferredWidth: getMaxRowWidth()
        //        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
        Layout.maximumWidth: legendFlow.width

        Repeater {
            id: repeater

            ItemDelegate {
                id: item
                implicitHeight: implicitContentHeight + topPadding + bottomPadding

                readonly property var series: chartView.series(index)

                highlighted: !series.visible


                topInset: 0; bottomInset: 0

                topPadding: font.pixelSize * 0.3; bottomPadding: font.pixelSize * 0.3
                leftPadding: font.pixelSize * 0.5; rightPadding: font.pixelSize * 0.5

                hoverEnabled: legendFlow.hoverEnabled

                onClicked: {
                    series.visible = highlighted
                    legendFlow.clicked(index)
                }

                onDoubleClicked: legendFlow.doubleClicked(index)

                //            Binding {
                //                target: series
                //                when: item.hovered
                //                property: "width"
                //                value: 4
                //            }

                contentItem: RowLayout {
                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: font.pixelSize * 0.6
                        Layout.preferredHeight: font.pixelSize * 0.6
                        radius: 2
                        color: series.color
                        //                    border.width: 1
                    }

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.maximumWidth: font.pixelSize * 12
                        elide: Qt.ElideLeft
                        text: series.name
                    }

                    Label {
                        id: removeButton
                        text: "×"
                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillHeight: true
                        enabled: removable
                        visible: item.hovered && enabled
                        color: "red"
                        background: MouseArea {
                            onClicked: legendFlow.removeClicked(index)
                        }
                    }
                }

            }
        }
    }
}
*/






/*
Flow {
    id: legendFlow
    property var chartView: null

    property bool hoverEnabled: true
    property bool removable: false

    signal clicked(int index)
    signal doubleClicked(int index)
    signal removeClicked(int index)

    Connections {
        target: chartView
        onSeriesAdded: repeater.model = chartView.count
        onSeriesRemoved: repeater.model = chartView.count
    }

    Repeater {
        id: repeater

        ItemDelegate {
            id: item
            implicitHeight: implicitContentHeight + topPadding + bottomPadding

            readonly property var series: chartView.series(index)

            highlighted: !series.visible


            topInset: 0; bottomInset: 0

            topPadding: font.pixelSize * 0.3; bottomPadding: font.pixelSize * 0.3
            leftPadding: font.pixelSize * 0.5; rightPadding: font.pixelSize * 0.5

            hoverEnabled: legendFlow.hoverEnabled

            onClicked: {
                series.visible = highlighted
                legendFlow.clicked(index)
            }

            onDoubleClicked: legendFlow.doubleClicked(index)

            //            Binding {
            //                target: series
            //                when: item.hovered
            //                property: "width"
            //                value: 4
            //            }

            contentItem: RowLayout {
                Rectangle {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: font.pixelSize * 0.6
                    Layout.preferredHeight: font.pixelSize * 0.6
                    radius: 2
                    color: series.color
                    //                    border.width: 1
                }

                Label {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.maximumWidth: font.pixelSize * 12
                    elide: Qt.ElideLeft
                    text: series.name
                }

                Label {
                    id: removeButton
                    text: "×"
                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillHeight: true
                    enabled: removable
                    visible: item.hovered && enabled
                    color: "red"
                    background: MouseArea {
                        onClicked: legendFlow.removeClicked(index)
                    }
                }
            }

        }
    }
}*/
