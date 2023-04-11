import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QCC.Dialogs 1.0 as QD
import QtCharts 2.3
import QCC.DataView.ChartViewExtension 1.0

Pane {
    id: form

    property real rowSpacing: 5
    property real columnSpacing: 0

    spacing: 10
    property var chartView: null

    Component.onDestruction: colorDialogLoader.active = false

    property bool horizontalAxisActive: true

    contentItem: Flickable {
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        interactive: contentHeight > height
        contentHeight: column.height

        clip: true
        Column {
            id: column
            width: parent.width
            spacing: form.spacing

            GroupBox {
                id: chartSettingsGroupBox
                // chart settings
                //                title: qsTr("Chart settings")

                width: parent.width
                contentItem: GridLayout {
                    rowSpacing: form.rowSpacing
                    columnSpacing: form.columnSpacing

                    /*
                    RowLayout {
                        id: row
                        Layout.column: 0; Layout.row: 0
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        spacing: form.rowSpacing

                        property real linePadding: 3

                        ItemDelegate {
                            id: itemm
                            Layout.fillWidth: true; Layout.preferredHeight: 20
                            padding: 0
                            topPadding: 0; bottomPadding: 0


                            contentItem: Item {
                                Rectangle {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: Math.min(parent.width, parent.height); height: width
                                    Rectangle { width: 2; height: parent.height - 2 * row.linePadding; x: row.linePadding; color: "#ADADAD" }
                                    Rectangle { width: parent.width - 2 * row.linePadding; height: 2; x: row.linePadding; y: parent.height - row.linePadding - 1; color: "#ADADAD" }
                                }


                            }
                        }
                    }
                    */

                    Label {
                        text: qsTr("Axis type")
                        Layout.column: 0; Layout.row: 0
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    ComboBox {
                        Layout.column: 1; Layout.row: 0
                        Layout.preferredWidth: 3; Layout.fillWidth: true
                        model: horizontalAxisActive ? [ qsTr("One axis"), qsTr("Multi vertical axes"), qsTr("Multi horizontal axes"), qsTr("Multi axes") ]
                                                    : [ qsTr("One axis"), qsTr("Multi vertical axes") ]

                        function typeToIndex(type) {
                            switch(type) {
                            case ChartViewExtension.OneAxis: return 0
                            case ChartViewExtension.MultipleVerticalAxes: return 1
                            case ChartViewExtension.MultipleHorizontalAxes: return 2
                            case ChartViewExtension.MultipleAxes: return 3
                            default: break
                            }
                            return 0
                        }

                        function indexToType(index) {
                            switch(index) {
                            case 0: return ChartViewExtension.OneAxis
                            case 1: return ChartViewExtension.MultipleVerticalAxes
                            case 2: return ChartViewExtension.MultipleHorizontalAxes
                            case 3: return ChartViewExtension.MultipleAxes
                            default: break
                            }
                            return ChartViewExtension.OneAxis
                        }

                        currentIndex: chartView === null ? 0
                                                         : typeToIndex(chartView.chartAxesType)

                        onActivated: {
                            if(chartView === null) return
                            chartView.chartAxesType = indexToType(currentIndex)
                        }
                    }



                    /*

                    Label {
                        text: qsTr("Theme")
                        Layout.column: 0; Layout.row: 1
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    ComboBox {
                        Layout.column: 1; Layout.row: 1
                        Layout.preferredWidth: 3; Layout.fillWidth: true
                        model: [ qsTr("Light"), qsTr("BlueCerulean"), qsTr("Dark"),
                            qsTr("BrownSand"), qsTr("BlueNcs"), qsTr("HighContrast"),
                            qsTr("Qt")/]

                        function themeToIndex(type) {
                            switch(type) {
                            case ChartView.ChartThemeLight: return 0
                            case ChartView.ChartThemeBlueCerulean: return 1
                            case ChartView.ChartThemeDark: return 2
                            case ChartView.ChartThemeBrownSand: return 3
                            case ChartView.ChartThemeBlueNcs: return 4
                            case ChartView.ChartThemeHighContrast: return 5
                            case ChartView.ChartThemeQt: return 6
                            default: break
                            }
                            return 0
                        }

                        function indexToTheme(index) {
                            switch(index) {
                            case 0: return ChartView.ChartThemeLight
                            case 1: return ChartView.ChartThemeBlueCerulean
                            case 2: return ChartView.ChartThemeDark
                            case 3: return ChartView.ChartThemeBrownSand
                            case 4: return ChartView.ChartThemeBlueNcs
                            case 5: return ChartView.ChartThemeHighContrast
                            case 6: return ChartView.ChartThemeQt
                            default: break
                            }
                            return ChartView.ChartThemeLight
                        }

                        //                        currentIndex: chartView === null ? 0
                        //                                                         : themeToIndex(chartView.theme)

                        onActivated: {
                            if(chartView === null) return
                            chartView.theme = indexToTheme(currentIndex)
                        }
                    }
                    */

                    Label {
                        text: qsTr("Use OpenGL")
                        Layout.column: 0; Layout.row: 1
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    ComboBox {
                        Layout.column: 1; Layout.row: 1
                        Layout.preferredWidth: 3; Layout.fillWidth: true
                        model: [ qsTr("Use"), qsTr("Not use"), qsTr("Auto") ]

                        function typeToIndex(type) {
                            switch(type) {
                            case ChartViewExtension.Use: return 0
                            case ChartViewExtension.NotUse: return 1
                            case ChartViewExtension.Auto: return 2
                            default: break
                            }
                            return 0
                        }

                        function indexToType(index) {
                            switch(index) {
                            case 0: return ChartViewExtension.Use
                            case 1: return ChartViewExtension.NotUse
                            case 2: return ChartViewExtension.Auto
                            default: break
                            }
                            return ChartViewExtension.Auto
                        }

                        currentIndex: chartView === null ? 0
                                                         : typeToIndex(chartView.openGlUsage)

                        onActivated: {
                            if(chartView === null) return
                            chartView.openGlUsage = indexToType(currentIndex)
                        }
                    }

                    //                    Label {
                    //                        text: qsTr("Averaging interval")
                    //                        Layout.column: 0; Layout.row: 2
                    //                        Layout.preferredWidth: 2; Layout.fillWidth: true
                    //                        wrapMode: Label.WordWrap
                    //                    }

                    //                    TextField {
                    //                        Layout.column: 1; Layout.row: 2
                    //                        Layout.preferredWidth: 3; Layout.fillWidth: true
                    //                        text: chartView === null ? ""
                    //                                                 : chartView.averagingInterval
                    //                        validator: IntValidator {
                    //                            bottom: 1
                    //                        }

                    //                        onEditingFinished: chartView.averagingInterval = text
                    //                    }

                    // Series type..
                }
            }   // GroupBox


            GroupBox {
                id: lineSeriesSettingsGroupBox
                // line series settings
                width: parent.width
                visible: seriesComboBox.currentSeries !== null

                //                title: qsTr("Series settings")

                contentItem: GridLayout {
                    rowSpacing: form.rowSpacing
                    columnSpacing: form.columnSpacing

                    RowLayout {
                        spacing: form.rowSpacing

                        Layout.fillWidth: true
                        Layout.column: 0; Layout.row: 0
                        Layout.columnSpan: 2

                        ComboBox {
                            id: seriesComboBox
                            Layout.fillWidth: true

                            Connections {
                                target: chartView
                                onSeriesAdded: seriesComboBox.model = chartView.count
                                onSeriesRemoved: seriesComboBox.model = chartView.count
                            }

                            currentIndex: 0

                            property var currentSeries: model === null ? null
                                                                       : chartView == null ? ""
                                                                                           : chartView.series(currentIndex)
                            displayText: currentSeries === null ? ""
                                                                : currentSeries.name
                            delegate: ItemDelegate {
                                width: parent.width
                                text: chartView.series(index).name
                                highlighted: seriesComboBox.currentIndex === index
                            }
                        }

                        Button {
                            id: colorButton
                            Layout.fillWidth: true; Layout.fillHeight: true

                            background: Rectangle {
                                radius: 2
                                border.color: "#4A4A4A"
                                color: seriesComboBox.currentSeries === null ? "white"
                                                                             : colorButton.hovered ? Qt.darker(seriesComboBox.currentSeries.color, 1.2)
                                                                                                   : seriesComboBox.currentSeries.color
                            }

                            onClicked: colorDialogLoader.active = true
                        }
                    }   // RowLayout series


                    // Visible
                    Label {
                        text: qsTr("Visible")
                        Layout.column: 0; Layout.row: 1
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    RowLayout {
                        Layout.column: 1; Layout.row: 1
                        Layout.preferredWidth: 3; Layout.fillWidth: true

                        RadioButton {
                            text: qsTr("Yes")
                            checked: seriesComboBox.currentSeries === null ? false
                                                                           : seriesComboBox.currentSeries.visible
                            onClicked: seriesComboBox.currentSeries.visible = true
                        }

                        RadioButton {
                            text: qsTr("No")
                            checked: seriesComboBox.currentSeries === null ? true
                                                                           : !seriesComboBox.currentSeries.visible
                            onClicked: seriesComboBox.currentSeries.visible = false
                        }
                    }   // RowLayout group buttons

                    // Opacity
                    Label {
                        text: qsTr("Opacity")
                        Layout.column: 0; Layout.row: 2
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    Slider {
                        Layout.column: 1; Layout.row: 2
                        Layout.preferredWidth: 3; Layout.fillWidth: true

                        from: 0
                        to: 1.0
                        stepSize: 0.05
                        value: seriesComboBox.currentSeries === null ? 0
                                                                     : seriesComboBox.currentSeries.opacity
                        onMoved: seriesComboBox.currentSeries.opacity = value
                    }

                    // Width
                    Label {
                        text: qsTr("Width")
                        Layout.column: 0; Layout.row: 3
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    //                    ComboBox {
                    //                        Layout.column: 1; Layout.row: 3
                    //                        Layout.preferredWidth: 3; Layout.fillWidth: true
                    //                        model: 15
                    //                        currentIndex: seriesComboBox.currentSeries === null ? 0
                    //                                                                            : seriesComboBox.currentSeries.width
                    //                        onActivated: seriesComboBox.currentSeries.width = currentIndex
                    //                    }

                    Slider {
                        Layout.column: 1; Layout.row: 3
                        Layout.preferredWidth: 3; Layout.fillWidth: true

                        from: 1
                        to: 10
                        stepSize: 1
                        value: seriesComboBox.currentSeries === null ? 0
                                                                     : seriesComboBox.currentSeries.width
                        onMoved: seriesComboBox.currentSeries.width = value
                    }

                    // Style
                    Label {
                        text: qsTr("Style")
                        Layout.column: 0; Layout.row: 4
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    ComboBox {
                        Layout.column: 1; Layout.row: 4
                        Layout.preferredWidth: 3; Layout.fillWidth: true

                        model: [ qsTr("Plain line"), qsTr("Dashes separated by a few pixels"), qsTr("Dots separated by a few pixels"),
                            qsTr("Alternate dots and dashes"), qsTr("One dash, two dots, one dash, two dots") ]

                        function styleToIndex(type) {
                            switch(type) {
                            case Qt.SolidLine: return 0
                            case Qt.DashLine: return 1
                            case Qt.DotLine: return 2
                            case Qt.DashDotLine: return 3
                            case Qt.DashDotDotLine: return 4
                            default: break
                            }
                            return 0
                        }

                        function indexToStyle(index) {
                            switch(index) {
                            case 0: return Qt.SolidLine
                            case 1: return Qt.DashLine
                            case 2: return Qt.DotLine
                            case 3: return Qt.DashDotLine
                            case 4: return Qt.DashDotDotLine
                            default: break
                            }
                            return Qt.SolidLine
                        }

                        currentIndex: seriesComboBox.currentSeries === null ? 0
                                                                            : styleToIndex(seriesComboBox.currentSeries.style)

                        onActivated: {
                            if(seriesComboBox.currentSeries === null) return
                            seriesComboBox.currentSeries.style = indexToStyle(currentIndex)
                        }
                    }


                    // PointsVisible
                    Label {
                        text: qsTr("Points visible")
                        Layout.column: 0; Layout.row: 5
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }


                    RowLayout {
                        Layout.column: 1; Layout.row: 5
                        Layout.preferredWidth: 3; Layout.fillWidth: true

                        Connections {
                            // Тк нет сигнала изменения pointsVisible
                            target: seriesComboBox
                            onCurrentSeriesChanged: {
                                pointsVisibleButton.checked = seriesComboBox.currentSeries === null ? false
                                                                                                    : seriesComboBox.currentSeries.pointsVisible
                                pointsInvisibleButton.checked = !pointsVisibleButton.checked
                            }
                        }

                        RadioButton {
                            id: pointsVisibleButton
                            text: qsTr("Yes")
                            onClicked: seriesComboBox.currentSeries.pointsVisible = true
                        }

                        RadioButton {
                            id: pointsInvisibleButton
                            text: qsTr("No")
                            onClicked: seriesComboBox.currentSeries.pointsVisible = false
                        }
                    }   // RowLayout group buttons


                    // pointLabelsVisible
                    Label {
                        text: qsTr("Point labels visible")
                        Layout.column: 0; Layout.row: 6
                        Layout.preferredWidth: 2; Layout.fillWidth: true
                        wrapMode: Label.WordWrap
                    }

                    RowLayout {
                        Layout.column: 1; Layout.row: 6
                        Layout.preferredWidth: 3; Layout.fillWidth: true

                        RadioButton {
                            text: qsTr("Yes")
                            checked: seriesComboBox.currentSeries === null ? false
                                                                           : seriesComboBox.currentSeries.pointLabelsVisible
                            onClicked: seriesComboBox.currentSeries.pointLabelsVisible = true
                        }

                        RadioButton {
                            text: qsTr("No")
                            checked: seriesComboBox.currentSeries === null ? true
                                                                           : !seriesComboBox.currentSeries.pointLabelsVisible
                            onClicked: seriesComboBox.currentSeries.pointLabelsVisible = false
                        }
                    }   // RowLayout group buttons




                    //                    Label {
                    //                        text: qsTr("Min x")
                    //                        Layout.column: 0; Layout.row: 7
                    //                        Layout.preferredWidth: 2; Layout.fillWidth: true
                    //                        wrapMode: Label.WordWrap
                    //                    }

                    //                    TextField {
                    //                        Layout.column: 1; Layout.row: 7
                    //                        Layout.preferredWidth: 3; Layout.fillWidth: true
                    //                        text: seriesComboBox.currentSeries === null ? false
                    //                                                                    : chartView.axisX(seriesComboBox.currentSeries).min
                    //                    }

                    //                    Label {
                    //                        text: qsTr("Max x")
                    //                        Layout.column: 0; Layout.row: 8
                    //                        Layout.preferredWidth: 2; Layout.fillWidth: true
                    //                        wrapMode: Label.WordWrap
                    //                    }

                    //                    TextField {
                    //                        Layout.column: 1; Layout.row: 8
                    //                        Layout.preferredWidth: 3; Layout.fillWidth: true
                    //                        text: seriesComboBox.currentSeries === null ? false
                    //                                                                    : chartView.axisX(seriesComboBox.currentSeries).max
                    //                    }


                    //                    Label {
                    //                        text: qsTr("Min y")
                    //                        Layout.column: 0; Layout.row: 9
                    //                        Layout.preferredWidth: 2; Layout.fillWidth: true
                    //                        wrapMode: Label.WordWrap
                    //                    }

                    //                    TextField {
                    //                        Layout.column: 1; Layout.row: 9
                    //                        Layout.preferredWidth: 3; Layout.fillWidth: true
                    //                        text: seriesComboBox.currentSeries === null ? false
                    //                                                                    : chartView.axisY(seriesComboBox.currentSeries).min
                    //                    }

                    //                    Label {
                    //                        text: qsTr("Max y")
                    //                        Layout.column: 0; Layout.row: 10
                    //                        Layout.preferredWidth: 2; Layout.fillWidth: true
                    //                        wrapMode: Label.WordWrap
                    //                    }

                    //                    TextField {
                    //                        Layout.column: 1; Layout.row: 10
                    //                        Layout.preferredWidth: 3; Layout.fillWidth: true
                    //                        text: seriesComboBox.currentSeries === null ? false
                    //                                                                    : chartView.axisY(seriesComboBox.currentSeries).max
                    //                    }

                    //                    Button {
                    //                        Layout.column: 0; Layout.row: 11
                    //                        Layout.columnSpan: 2
                    //                        Layout.fillWidth: true
                    //                        text: qsTr("Autoscale")
                    //                    }




                }   // RowLayout

            }   // GroupBox



            //            GroupBox {
            //                width: parent.width
            //                contentItem: GridLayout {
            //                    rowSpacing: form.rowSpacing
            //                    columnSpacing: form.columnSpacing

            //                    Label {
            //                        text: qsTr("Label font size")
            //                        Layout.column: 0; Layout.row: 0
            //                        Layout.preferredWidth: 2; Layout.fillWidth: true
            //                        wrapMode: Label.WordWrap
            //                    }

            //                    Slider {
            //                        Layout.column: 1; Layout.row: 0
            //                        Layout.preferredWidth: 3; Layout.fillWidth: true

            ////                        from: 1
            ////                        to: 10
            ////                        stepSize: 1
            ////                        value: seriesComboBox.currentSeries === null ? 0
            ////                                                                     : seriesComboBox.currentSeries.width
            ////                        onMoved: seriesComboBox.currentSeries.width = value
            //                    }


            // legend size


            //                }
            //            }   // GroupBox

        }   // column
    }   // Flickable


    Loader {
        id: colorDialogLoader
        active: false

        sourceComponent: QD.ColorDialog {
            title: qsTr("Color selection")
            onAccepted: {
                seriesComboBox.currentSeries.color = color
                colorDialogLoader.active = false
            }
            onRejected: {
                colorDialogLoader.active = false
            }
        }
    }   // colorDialogLoader

}
