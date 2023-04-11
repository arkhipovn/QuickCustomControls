import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import QCC.DataView.TableView 1.0
import QCC.DataView.TableView.Private 1.0 as TV
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

Pane {
    id: tableBackground

    topPadding: 1; bottomPadding: 1
    leftPadding: 1; rightPadding: 1

    property int elevation: 0

    function printTable(fullTable = false) {
        selectionModel.printTable(verticalHeaderItem.getHeaderData(), horizontalHeader.getHeaderData(), fullTable)
    }

    //@N что делать если не стоит флаг using_widgets
    function saveToCSV(fullTable = false) {
        selectionModel.saveToCSV(verticalHeaderItem.getHeaderData(), horizontalHeader.getHeaderData(), fullTable)
    }

    background: Rectangle {
        border.color: borderColor
        layer.enabled: tableBackground.enabled && tableBackground.elevation > 0
        layer.effect: ElevationEffect {
            elevation: Math.max(tableBackground.elevation, tableBackground.Material.elevation)
        }
    }

    //focusPolicy: Qt.StrongFocus
    // это как отображать. Где будут колонки, где строки. Qt.Hor - таблица будет перевернута
    //    property var orientation: Qt.Vertical

    property alias roleProvider: selectionModel.roleProvider
    property string backgroundRole: "background"

    property string dateFormat: "dd.MM.yyyy HH:mm:ss"

    property alias model: table.model

    readonly property alias rows: table.rows
    readonly property alias columns: table.columns

    property alias horizontalHeader: horizontalHeaderItem
    property alias verticalHeader: verticalHeaderItem

    // так сделал из-за флага stretchLastSection
    //    property var columnWidthProvider: function () { return font.pixelSize * 10; }
    //    property var rowHeightProvider: function () { return font.pixelSize * 3; }

    property var cellTextProvider: function (column, value) {
        return value instanceof Date ? Qt.formatDateTime(value, dateFormat)
                                     : value;
    }

    property alias columnWidthProvider: horizontalHeaderItem.sizeProvider
    property alias rowHeightProvider: verticalHeaderItem.sizeProvider

    property alias verticalHeaderWidth: verticalHeaderItem.cellWidth
    property alias horizontalHeaderHeight: horizontalHeaderItem.cellHeight

    property alias verticalHeaderDataProvider: verticalHeaderItem.headerDataProvider
    property alias horizontalHeaderDataProvider: horizontalHeaderItem.headerDataProvider

    activeFocusOnTab: true
    onFocusChanged: if(focus) tableBackground.forceActiveFocus()

    property color borderColor: Qt.rgba(textColor.r, textColor.g, textColor.b, 0.10);
    property color mainColor: "#878787"//"#3F51B5"
    property color sideColor: "white"
    property color textColor: "black"
    property color selectedColor: "#0057A3"

    property alias selectedIndexes: selectionModel.selectedIndexes

    function getSelectedIndexes() { return selectionModel.selectedIndexes }
    function clearSelection() { selectionModel.clear() }

    property alias stretchLastSection: horizontalHeaderItem.stretchLastSection

    signal contextMenuRequested(var mouseX, var mouseY)
    signal doubleClicked(var mouseX, var mouseY, var modelIndex)

    readonly property alias selectionModel: selectionModel

    property alias selectionMode: selectionModel.selectionMode
    property alias selectionBehavior: selectionModel.selectionBehavior

    property alias sortIndicatorVisible: horizontalHeaderItem.sortIndicatorVisible
    property alias sortIndicatorColumn: horizontalHeaderItem.sortIndicatorColumn
    //    AscendingOrder,
    //    DescendingOrder
    property alias sortIndicatorOrder: horizontalHeaderItem.sortIndicatorOrder

    //    onSortIndicatorColumnChanged: sort()
    //    onSortIndicatorOrderChanged: sort()

    //    function sort() {
    //        if(!sortIndicatorVisible)
    //            return
    //        if(table.model == null)
    //            return
    //        table.model.sort(sortIndicatorColumn, sortIndicatorOrder)
    //    }

    property int wrapMode: Label.NoWrap
    property int elide: wrapMode == Label.NoWrap ? Label.ElideRight
                                                 : Label.ElideNone

    property var toolTip: null

    property Component delegate: Component {
        Label {
            text: styleData.value === undefined ? ""
                                                : cellTextProvider === null || cellTextProvider === undefined ? styleData.value
                                                                                                              : cellTextProvider(styleData.columnIndex, styleData.value)
            color: styleData.textColor
            wrapMode: tableBackground.wrapMode
            verticalAlignment: Label.AlignVCenter
            padding: font.pixelSize
            elide: tableBackground.elide

            // выделение
            Rectangle {
                width: parent.width; height: parent.height
                opacity: styleData.selected ? 0.2 : 0.0
                color: styleData.currentIndexSelectedItem === styleData.currentIndex ? styleData.textColor : styleData.selectedColor
            }
        }
    }

    contentItem: Item {
        clip: true

        TV.HeaderView {
            id: horizontalHeaderItem
            tableView: table
            selectionModel: selectionModel
            orientation: Qt.Horizontal
            //        orientation: tableBackground.orientation == Qt.Vertical ? Qt.Horizontal : Qt.Vertical
            //        sizeProvider: tableBackground.columnWidthProvider

            borderColor: tableBackground.borderColor
            mainColor: tableBackground.mainColor
            textColor: tableBackground.textColor
        }   // HeaderView

        TV.HeaderView {
            id: verticalHeaderItem
            orientation: Qt.Vertical //tableBackground.orientation == Qt.Vertical ? Qt.Vertical : Qt.Horizontal

            tableView: table
            selectionModel: selectionModel

            //        sizeProvider: tableBackground.rowHeightProvider

            borderColor: tableBackground.borderColor
            mainColor: tableBackground.mainColor
            textColor: tableBackground.textColor
        }   // HeaderView

        // левый верхний угол
        TV.TableCell {
            visible: verticalHeaderItem.visible
            z: 3
            width: table.x; height: table.y
            border.color: tableBackground.borderColor
            color: Qt.rgba(tableBackground.mainColor.r, tableBackground.mainColor.g, tableBackground.mainColor.b, 0.20)

            MouseArea {
                width: parent.width; height: parent.height
                onClicked: selectionModel.selectAll()
            }
        }

        TableView {
            id: table
            anchors { top: horizontalHeader.bottom; bottom: parent.bottom; left: verticalHeader.right; right: parent.right; }
            clip: true
            focus: true

            //SingleSelection
            boundsBehavior: TableView.StopAtBounds

            ScrollBar.horizontal: ScrollBar { id: horizontalScrollBarItem; policy: table.contentWidth > table.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff; }
            ScrollBar.vertical: ScrollBar { id: verticalScrollBarItem; policy: table.contentHeight > table.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff; }

            onRowsChanged: selectionModel.clear()
            onColumnsChanged: selectionModel.clear()

            columnWidthProvider: horizontalHeaderItem.currentSizeProvider
            rowHeightProvider: verticalHeaderItem.currentSizeProvider

            onWidthChanged: table.forceLayout();
            //        onHeightChanged: table.forceLayout();

            interactive: !selectionMouse.pressed

            delegate: Loader {
                active: width != 0 && height != 0
                sourceComponent: TV.TableCell {
                    readonly property var backgroundColor: model[backgroundRole]
                    color: backgroundColor != undefined ? backgroundColor :
                                                          row & 1 ? Qt.rgba(tableBackground.mainColor.r, tableBackground.mainColor.g, tableBackground.mainColor.b, 0.10)
                                                                  : tableBackground.sideColor

                    border.color: tableBackground.borderColor

                    Loader {
                        width: parent.width; height: parent.height

                        // эти свойства предоставляются делегату строки
                        property QtObject styleData: QtObject {
                            readonly property int rowIndex: row
                            readonly property int columnIndex: column
                            readonly property string role: tableBackground.roleProvider(columnIndex)
                            readonly property var value: model.hasOwnProperty(role) ? model[role] // Qml ListModel and QAbstractItemModel
                                                                                    : modelData && modelData.hasOwnProperty(role)
                                                                                      ? modelData[role] // QObjectList / QObject
                                                                                      : modelData != undefined ? modelData : "" // Models without role
                            readonly property var currentIndex: table.model == null ? -1
                                                                                    : table.model.index(row, column)
                            readonly property bool selected: selectionModel.hasSelection ? selectionModel.isSelected(currentIndex) : false

                            readonly property var currentIndexSelectedItem: selectionModel.currentIndex

                            readonly property color textColor: tableBackground.textColor
                            readonly property color selectedColor: tableBackground.selectedColor
                        }

                        sourceComponent: tableBackground.delegate
                    }
                }
            }

            SelectionModel {
                id: selectionModel
                roleProvider: function () { return "display"; }
                model: selectionModel.selectionMode === SelectionModel.NoSelection ? null:
                                                                                     table.model === 0 ? null :
                                                                                                         table.model
                property var lastSelectedIndex: null
                function setLastSelectedIndex(index) {
                    if(model == null) return
                    if(selectionModel.lastSelectedIndex !== index) {
                        selectionModel.lastSelectedIndex = index
                    }
                }

                onLastSelectedIndexChanged: {
                    if(model == null) return
                    if(lastSelectedIndex == null) return

                    // найдем координаты начала и конца
                    var startPos = Qt.point(horizontalHeaderItem.getStartPos(lastSelectedIndex.column), verticalHeaderItem.getStartPos(lastSelectedIndex.row))
                    var endPos = Qt.point(horizontalHeaderItem.getEndPos(lastSelectedIndex.column), verticalHeaderItem.getEndPos(lastSelectedIndex.row))

                    if(endPos.x > table.contentX + table.width)
                        table.contentX = endPos.x - table.width
                    else if(startPos.x < table.contentX)
                        table.contentX = startPos.x

                    if(endPos.y > table.contentY + table.height)
                        table.contentY = endPos.y - table.height
                    else if(startPos.y < table.contentY)
                        table.contentY = startPos.y
                }
            }   // SelectionModel

            MouseArea {
                id: selectionMouse
                width: parent.width; height: parent.height
                enabled: table.model != null && (selectionModel.selectionMode !== SelectionModel.NoSelection || toolTip != null)
                propagateComposedEvents: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                hoverEnabled: toolTip != null      // || selectionModel.lastSelectedIndex != null

                onExited: if(toolTip != null) toolTip.visible = false

                onPressed: {
                    if(toolTip != null) toolTip.visible = false

                    if(selectionModel.selectionMode === SelectionModel.NoSelection)
                        return

                    if(mouse.button & Qt.RightButton) return;
                    if(!tableBackground.focus) tableBackground.focus = true

                    var row = verticalHeaderItem.headerIndexAt(mouseY)
                    var column = horizontalHeaderItem.headerIndexAt(mouseX)

                    if(row < 0 || column < 0) return
                    var index = table.model.index(row, column)
                    var controlModifier = (mouse.modifiers & Qt.ControlModifier) ? true : false

                    if(mouse.modifiers & Qt.ShiftModifier && selectionModel.currentIndex.valid)  // выделить область нужно
                        selectionModel.selection(selectionModel.currentIndex, index)
                    else selectionModel.setCurrentIndex(index, !controlModifier)
                    selectionModel.setLastSelectedIndex(index)
                }

                onPositionChanged: {
                    if(!pressed) {
                        if(toolTip != null) {
                            let toolTipRow = verticalHeaderItem.headerIndexAt(mouseY, false)
                            let toolTipColumn = horizontalHeaderItem.headerIndexAt(mouseX, false)
                            if(toolTipRow !== -1 && toolTipColumn !== -1) {
                                var cellText = cellTextProvider(toolTipColumn,
                                                                table.model.data(table.model.index(toolTipRow, toolTipColumn),
                                                                                 tableBackground.roleProvider(toolTipColumn)))

                                toolTip.visible = cellText !== undefined && cellText.length > 0
                                toolTip.text = cellText === undefined ? ""
                                                                      : cellText
                            }
                        }
                    }
                    else if(selectionModel.selectionMode !== SelectionModel.NoSelection) {
                        var row = verticalHeaderItem.headerIndexAt(mouseY)
                        var column = horizontalHeaderItem.headerIndexAt(mouseX)

                        if(row < 0) {
                            if(mouseY < 0) row = 0
                            else if(mouseY > table.height) row = table.rows - 1
                        }
                        if(column < 0) {
                            if(mouseX < 0) column = 0
                            else if(mouseX > table.width) column = table.columns - 1
                        }

                        var index = table.model.index(row, column)
                        selectionModel.selection(selectionModel.currentIndex, index)
                        selectionModel.setLastSelectedIndex(index)
                    }
                }

                onClicked: {
                    if(selectionModel.selectionMode === SelectionModel.NoSelection)
                        return

                    if(mouse.button & Qt.RightButton) {
                        if(selectionModel.selectedIndexes.length === 0) {
                            // если ничего нет и нажали правой кнопкой, то выделим

                            var row = verticalHeaderItem.headerIndexAt(mouseY)
                            var column = horizontalHeaderItem.headerIndexAt(mouseX)
                            if(row >= 0 && column >= 0) {
                                var index = table.model.index(row, column)
                                selectionModel.setCurrentIndex(index)
                            }
                        }
                        tableBackground.contextMenuRequested(mouseX - table.contentX,
                                                             mouseY - table.contentY)
                    }
                }

                onDoubleClicked: {
                    if(selectionModel.selectionMode === SelectionModel.NoSelection)
                        return

                    var ss = selectionModel.selectedIndexes
                    if(ss.length > 0) tableBackground.doubleClicked(mouseX - table.contentX,
                                                                    mouseY - table.contentY,
                                                                    ss[0])
                }
            }   // MouseArea

            Keys.onPressed: {
                var controlModifier = (event.modifiers & Qt.ControlModifier) ? true : false
                var shiftModifier = (event.modifiers & Qt.ShiftModifier) ? true : false

                if((event.key === Qt.Key_A) && controlModifier) selectionModel.selectAll()
                if((event.key === Qt.Key_C) && controlModifier) {
                    print("copy")
                    selectionModel.copy(verticalHeaderItem.getHeaderData(), horizontalHeader.getHeaderData())
                }

                switch(event.key) {
                case Qt.Key_PageUp:
                    verticalScrollBarItem.decrease()
                    break
                case Qt.Key_PageDown:
                    verticalScrollBarItem.increase()
                    break
                case Qt.Key_Up:
                    selectionModel.decreaseRowIndex(!shiftModifier)
                    //                selectionModel.setLastSelectedIndex(selectionModel.currentIndex)
                    selectionModel.setLastSelectedIndex(table.model.index(selectionModel.currentIndex.row, selectionModel.currentIndex.column))
                    break
                case Qt.Key_Down:
                    selectionModel.increaseRowIndex(!shiftModifier)
                    //                selectionModel.setLastSelectedIndex(selectionModel.currentIndex)
                    selectionModel.setLastSelectedIndex(table.model.index(selectionModel.currentIndex.row, selectionModel.currentIndex.column))
                    break
                case Qt.Key_Left:
                    selectionModel.decreaseColumnIndex(!shiftModifier)
                    //                selectionModel.setLastSelectedIndex(selectionModel.currentIndex)
                    selectionModel.setLastSelectedIndex(table.model.index(selectionModel.currentIndex.row, selectionModel.currentIndex.column))
                    break
                case Qt.Key_Right:
                    selectionModel.increaseColumnIndex(!shiftModifier)
                    //                selectionModel.setLastSelectedIndex(selectionModel.currentIndex)
                    selectionModel.setLastSelectedIndex(table.model.index(selectionModel.currentIndex.row, selectionModel.currentIndex.column))
                    break
                default: break;
                }
            }

        }   // TableView
    }   // contentItem
}


