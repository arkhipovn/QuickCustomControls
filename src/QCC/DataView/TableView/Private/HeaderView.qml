import QtQuick 2.0
import QtQuick.Controls 2.12
import QCC.Fonts 1.0
import QCC.DataView.TableView 1.0

ListView {
    id: headerView
    orientation: Qt.Horizontal
    visible: tableView != null
    clip: true

    interactive: false

    property var headerDataProvider: function (index) {
        if(tableView == null) return undefined
        if(tableView.model === null || tableView.model === 0) return undefined
        return tableView.model.headerData(index, headerView.orientation)
    }

    property var selectionModel: null
    property var tableView: null
    onTableViewChanged: {
        // если изначально не скрыто
        if(visible) if(tableView == null) visible = false
    }

    property color mainColor: "#3F51B5"
    property color textColor: "black"
    property color borderColor: Qt.rgba(textColor.r, textColor.g, textColor.b, 0.10);

    property bool stretchLastSection: true  // работает только для Qt.Horizontal

    property bool sortIndicatorVisible: false
    property int sortIndicatorColumn: -1
    //    AscendingOrder,
    //    DescendingOrder

    property int sortIndicatorOrder: Qt.AscendingOrder

    property int cellWidth: font.pixelSize * 10;
    property int cellHeight: font.pixelSize * 3;

    property var sizeProvider: orientation === Qt.Horizontal ? (function () { return cellWidth; })
                                                             : (function () { return cellHeight; })

    readonly property int lastVisible: {
        if(sizeProvider !== null && orientation == Qt.Horizontal) {
            for(var i = count - 1; i >= 0; --i) {
                if(sizeProvider(i) !== 0)
                    return i
            }
        }
        return count - 1
    }

    property var currentSizeProvider: function (index) {
        if(sizeProvider === null || tableView == null) return
        if(orientation == Qt.Horizontal && (stretchLastSection && index === lastVisible) && tableView.width > startPosList[lastVisible]) {
            return Math.max(tableView.width - startPosList[lastVisible], sizeProvider(index))
        }
        return sizeProvider(index);
    }

    x: visible ? (orientation == Qt.Horizontal ? tableView.x : 0)
               : 0
    y: visible ? (orientation == Qt.Horizontal ? 0 : tableView.y)
               : 0

    width: visible ? (orientation == Qt.Horizontal ? tableView.width : cellWidth)
                   : 0

    height: visible ? (orientation == Qt.Horizontal ? cellHeight : tableView.height)
                    : 0

    contentX: visible ? (orientation == Qt.Horizontal ? tableView.contentX : 0)
                      : 0
    contentY: visible ? (orientation == Qt.Horizontal ? 0 : tableView.contentY)
                      : 0

    function getStartPos(index) {
        return index < count && index >= 0 ? startPosList[index] : -1
    }

    function getEndPos(index) {
        return index < count && index >= 0 ? startPosList[index] + currentSizeProvider(index): -1
    }

    function headerIndexAt(pos, behaviorCheck = true) {
        if(tableView == null) return -1
        if(selectionModel == null) return -1

        if(behaviorCheck && orientation == Qt.Vertical && selectionModel.selectionBehavior === SelectionModel.SelectColumns) return 0
        if(behaviorCheck && orientation == Qt.Horizontal && selectionModel.selectionBehavior === SelectionModel.SelectRows) return 0

        var startInd = 0
        var endInd = lastVisible
        while(startInd <= endInd) {
            var middle = Math.round((startInd + endInd) / 2)

            var startPos = getStartPos(middle)
            if(startPos > pos) endInd = middle - 1
            else {
                if(getEndPos(middle) <= pos) startInd = middle + 1
                else return middle
            }
        }

        return -1
    }

    property var startPosList: headerView.getStartPosList(true)
    Component.onCompleted: startPosList = Qt.binding(function() { return headerView.getStartPosList(false) })    // это сделаем после того, как станут известны размеры
    function getStartPosList(init) {
        if(tableView == null) return []
        var list = [0]
        var size = 0
        //@N SPACING
        for(var i = 1; i < count; ++i) {
            size += headerView.sizeProvider(i - 1)  //@N а как нужно все таки..
            //            size += init ? headerView.currentSizeProvider(i - 1) : headerView.sizeProvider(i - 1)
            list.push(size)
        }
        return list
    }

    function getHeaderData() {
        if(!visible || tableView == null) return []
        if(tableView.model === undefined || tableView.model === null) return []
        var list = []
        var count = orientation === Qt.Horizontal ? tableView.model.columnCount() : tableView.model.rowCount()
        for(var i = 0; i < count; ++i) {
            //            var t = tableView.model.headerData(i, orientation)
            var t = headerDataProvider(i)
            if(t === undefined) return []
            list.push(t)
        }
        return list;
    }

    model: tableView == null ? 0
                             : (orientation == Qt.Horizontal ? (tableView.columns > 0 ? tableView.columns : 0)
                                                             : (tableView.rows > 0 ? tableView.rows : 0))

    delegate: Loader {
        active: width != 0 && height != 0
        width: orientation === Qt.Horizontal ? headerView.currentSizeProvider(index) : cellWidth
        height: orientation === Qt.Horizontal ? cellHeight : headerView.currentSizeProvider(index)

        sourceComponent: TableCell {
            border.color: headerView.borderColor

            color: Qt.rgba(headerView.mainColor.r, headerView.mainColor.g, headerView.mainColor.b, 0.15)

            Label {
                anchors { left: parent.left; right: sortOrderIcon.active ? sortOrderIcon.left
                                                                         : parent.right; }
                height: parent.height

                text: {
                    var t = headerDataProvider(modelData)
                    return t === undefined ? index : t
                }
                wrapMode: Label.WordWrap
                color: headerView.textColor
                padding: font.pixelSize
                verticalAlignment: Text.AlignVCenter
            }

            Loader {
                id: sortOrderIcon
                anchors { right: parent.right; verticalCenter: parent.verticalCenter; }
                active: sortIndicatorVisible && sortIndicatorColumn == index// && sortingActive

                sourceComponent: Label {
                    padding: font.pixelSize
                    font.family: CFontLoader.awesome.name
                    text: sortIndicatorOrder == Qt.AscendingOrder ? "\uf0dd" : "\uf0de"
                }
            }
        }

    }

    MouseArea {
        width: parent.width; height: parent.height
        enabled: selectionModel != null && tableView != null && tableView.model !== 0
        hoverEnabled: !sortIndicatorVisible

        onPressed: {
            if(sortIndicatorVisible) {
                var index = orientation == Qt.Horizontal ? headerIndexAt(headerView.contentX + mouseX, false) : headerIndexAt(headerView.contentY + mouseY, false)
                if(sortIndicatorColumn !== index) {
                    // меняем колонку по которой идет сортировка
                    sortIndicatorColumn = index
                    sortIndicatorOrder = Qt.AscendingOrder
                }
                // меняем тип сортировки (по возр/ убыванию)
                else {
                    //                    if(sortingActive) {

                    //                    }
                    //                    else {
                    //                        sortIndicatorOrder = Qt.AscendingOrder
                    //                    }

                    sortIndicatorOrder = sortIndicatorOrder == Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder
                }
            }
            else {
                if(selectionModel == null) return
                if(selectionModel.model === null) return
                index = orientation == Qt.Horizontal ? headerIndexAt(headerView.contentX + mouseX, true) : headerIndexAt(headerView.contentY + mouseY, true)
                var controlModifier = (mouse.modifiers & Qt.ControlModifier) ? true : false
                orientation == Qt.Horizontal ? selectionModel.selectColumn(index, !controlModifier) : selectionModel.selectRow(index, !controlModifier)

                var row = orientation == Qt.Horizontal ? 0 : index
                var column = orientation == Qt.Horizontal ? index : 0
                selectionModel.setLastSelectedIndex(tableView.model.index(row, column))
            }
        }

        onPositionChanged: {
            if(!pressed) return
            if(sortIndicatorVisible) return
            if(selectionModel == null) return
            if(selectionModel.model === null) return

            var index = orientation == Qt.Horizontal ? headerIndexAt(headerView.contentX + mouseX, true) : headerIndexAt(headerView.contentY + mouseY, true)
            orientation == Qt.Horizontal ? selectionModel.selectColumn(index, false) : selectionModel.selectRow(index, false)

            var row = orientation == Qt.Horizontal ? 0 : index
            var column = orientation == Qt.Horizontal ? index : 0
            selectionModel.setLastSelectedIndex(tableView.model.index(row, column))
        }
    }
}

