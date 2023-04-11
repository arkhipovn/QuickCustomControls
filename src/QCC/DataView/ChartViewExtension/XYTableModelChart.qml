import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtCharts 2.3
import QCC.DataView.ChartViewExtension 1.0

XYChart {
    id: chartView
    antialiasing: true

    property alias model: seriesListMapper.model

    property alias xSectionProvider: seriesListMapper.xSectionProvider
    property alias ySectionProvider: seriesListMapper.ySectionProvider

    property alias xSectionRole: seriesListMapper.xSectionRole
    property alias ySectionRole: seriesListMapper.ySectionRole


    property alias averagingInterval: seriesListMapper.averagingInterval

    property int seriesType: ChartViewExtension.SeriesTypeLine


    property int openGlUsage: ChartViewExtension.Auto


    property int chartAxesType: ChartViewExtension.OneAxis

    //    property font font: Qt.application.font
    //    property real labelFontSize: undefined// Qt.application.font.pixelSize

    property Component axisXComponent: ValueAxis {
        tickCount: width > height ? 4 : 2
        minorTickCount: width > height ? 2 : 1
    }

    property Component axisYComponent: ValueAxis {
        minorTickCount: width > height ? 2 : 1
        //        labelFormat: "%g"
        //        labelsFont.pixelSize: chartView.font.pixelSize// labelFontSize
        //        labelFormat: "%d&deg;C"//"%4.2g"
    }

    onAxisXComponentChanged: updateSeriesList()
    onAxisYComponentChanged: updateSeriesList()


    onSeriesTypeChanged: updateSeriesList()

    onOpenGlUsageChanged: {
        for(var i = 0; i < count; ++i)
            useOpenGLFromSettings(series(i), openGlUsage)
    }
    onChartAxesTypeChanged: updateSeriesList()

    Component.onCompleted: updateSeriesList()


    onResetTransform: {    //@N пока не придумал как вернуть предыдущий масштаб.. Пока что по сигналу нужно вернуть назад все..
        for(var i = 0; i < count; ++i)
            setAxesDomain(i, axisX(series(i)), axisY(series(i)))
    }


    XYSeriesListMapper {
        id: seriesListMapper

        onUpdateSeriesList: chartView.updateSeriesList()   // Нужно удалить все предыдущие и создает новые линии

        onInsertSeries: { // Нужно добавить 1 или несколько линий
            // Проблема если добавили не в конец. Из-за того, что в
            // ChartView нет метода insert придется удалять часть и добавлять заново

            var lastCount = chartView.count

            // Сначала нужно удалить часть. Чтобы добавить новые между ними (Ну нет метода insert..)
            for(var i = lastCount - 1; i >= index; --i)
                eraseSeries(i)

            // А теперь уже нужно добавить
            for(i = index; i < lastCount + count; ++i)
                appendSeries(i, null)
        }

        onRemoveSeries: { // Нужно удалить одну или несколько линий
            var lastCount = chartView.count
            for(var i = index + count - 1; i >= index; --i)
                eraseSeries(i)
        }

        onReset: chartView.reset()  // Нужно очистить все линии
    }

    function reset() {
        removeAllSeries()
        seriesListMapper.removeAll()
        removeAllAxis() // @N мб и не нужно удалять? Как от них избавиться правильно?
    }

    function useOpenGLFromSettings(series, openGlUsage) {
        if(series === null) return
        switch(openGlUsage) {
        case ChartViewExtension.Use:
            series.useOpenGL = true
            break
        case ChartViewExtension.NotUse:
            series.useOpenGL = false
            break
        case ChartViewExtension.Auto:
        default:
            series.useOpenGL = Qt.binding(function() { return series.count > 1000 } )
            break
        }
    }

    function updateSeriesList() {
        //@N Если 0 или 1, то зачем что-то сохранять/менять/удалять? можно и так оставить..

        const numberOfLineInModel = seriesListMapper.numberOfLineInModel()

        var seriesSettingsList = []

        if(numberOfLineInModel === count) {
            // сохраним настройки
            for(let n = 0; n < count; ++n) {
                let s = series(n)
                var settings = { 'color': s.color.toString(16), 'width': s.width, 'style': s.style, 'visible': s.visible, 'opacity': s.opacity,
                    'pointLabelsVisible': s.pointLabelsVisible, 'pointsVisible': s.pointsVisible }
                seriesSettingsList.push(settings)
            }
        }

        reset() // Нужно очистить все, если что-то есть

        // Добавим новые
        for(let i = 0; i < numberOfLineInModel; ++i)
            appendSeries(i, (seriesSettingsList.length > i ? seriesSettingsList[i] : null))
    }

    function appendSeries(index, seriesSettings = null) {
        let s = createSeries(seriesType, "",
                             createAxis(Qt.Horizontal, index),
                             createAxis(Qt.Vertical, index))
        s.pointsReplaced.connect(function() { s.countChanged(s.count) })    //@N костыль.. Тк при вызове replace не отправляется сигнал изменения размера..

        //            s.clicked.connect( function() { print("clicked")})
        //            s.hovered.connect( function() { print("hovered")})
        //            s.doubleClicked.connect( function(point) { print("doubleClicked", point)})

        s.visibleChanged.connect(function() {
            switch(chartAxesType) {
            case ChartViewExtension.OneAxis:
                break
            case ChartViewExtension.MultipleHorizontalAxes:
                axisX(s).visible = s.visible
                break
            case ChartViewExtension.MultipleVerticalAxes:
                axisY(s).visible = s.visible
                break
            case ChartViewExtension.MultipleAxes:
                axisX(s).visible = s.visible
                axisY(s).visible = s.visible
                break
            default: break
            }

            seriesListMapper.updateDomain()
        })

        // Настр. line series
        useOpenGLFromSettings(s, openGlUsage)

        if(seriesSettings === null) {
            s.width = 2
        }
        else {
            s.width = seriesSettings.width
            s.style = seriesSettings.style
            s.visible = seriesSettings.visible
            s.opacity = seriesSettings.opacity
            s.color = seriesSettings.color
            s.pointLabelsVisible = seriesSettings.pointLabelsVisible
            s.pointsVisible = seriesSettings.pointsVisible
        }

        seriesListMapper.append(s)

        // Настр. размеры мин/макс и другое
        setAxesDomain(index, axisX(s), axisY(s))
        setTitleText(index, axisX(s), axisY(s))
    }


    //    onSeriesAdded: {
    //        print("added", series)
    //    }

    //    onSeriesRemoved: print("removed", series)

    function eraseSeries(index) {
        var s = series(index)
        if(s === null)
            return

        var axis_x = axisX(s)
        var axis_y = axisY(s)

        removeSeries(s)

        seriesListMapper.removeAt(index)
        removeAxis(Qt.Vertical, axis_y)
        removeAxis(Qt.Horizontal, axis_x)
    }

    function removeAxis(orientation, axis) {
        // Посмотрим нужно ли удалять оси
        switch(chartAxesType) {
        case ChartViewExtension.OneAxis:
            // удаляем только если count == 1
            if(count === 0)
                axis.destroy()
            break
        case ChartViewExtension.MultipleHorizontalAxes:
            if(orientation === Qt.Horizontal)
                axis.destroy()
            else if(count == 0)
                axis.destroy()
            break
        case ChartViewExtension.MultipleVerticalAxes:
            if(orientation === Qt.Vertical)
                axis.destroy()
            else if(count == 0)
                axis.destroy()
            break
        case ChartViewExtension.MultipleAxes:
            axis.destroy()
            break
        default: break
        }
    }

    function removeAllAxis() {
        //@N мб и не нужно удалять? Как от них избавиться правильно?
        for(var i = 0; i < axes.length; ++i)
            axes[i].destroy()
    }

    function createAxis(orientation, index) {
        if(index === 0) {
            return orientation === Qt.Vertical ? axisYComponent.createObject(chartView)
                                               : axisXComponent.createObject(chartView)
        }

        switch(chartAxesType) {
        case ChartViewExtension.MultipleHorizontalAxes:
            return orientation === Qt.Vertical ? axisY(series(0))
                                               : axisXComponent.createObject(chartView)

        case ChartViewExtension.MultipleVerticalAxes:
            return orientation === Qt.Vertical ? axisYComponent.createObject(chartView)
                                               : axisX(series(0))

        case ChartViewExtension.MultipleAxes:
            return orientation === Qt.Vertical ? axisYComponent.createObject(chartView)
                                               : axisXComponent.createObject(chartView)

        case ChartViewExtension.OneAxis:
        default:
            return orientation === Qt.Vertical ? axisY(series(0))
                                               : axisX(series(0))
        }
    }

    function setTitleText(index, axis_x, axis_y) {
        if(axis_x === null || axis_y === null)
            return

        switch(chartAxesType) {
        case ChartViewExtension.OneAxis:
            break
        case ChartViewExtension.MultipleHorizontalAxes:
            axis_x.titleText = Qt.binding(function() { return series(index).name; })
            break
        case ChartViewExtension.MultipleVerticalAxes:
            axis_y.titleText = Qt.binding(function() { return series(index).name; })
            break
        case ChartViewExtension.MultipleAxes:
            axis_y.titleText = Qt.binding(function() { return series(index).name; })
            axis_x.titleText = Qt.binding(function() { return series(index).name; })
            break
        default: break
        }
    }

    function updateAxisDomain(axis, domain, orientation) {
        // Конвертирует в нужный формат граничные значения
        // Не знаю как правильно сделать.. Почему нельзя получить тип...

        if(orientation === Qt.Vertical) {
            var offset = domain.minY === domain.maxY ? (domain.maxY === 0 ? 0.1 : Math.abs(domain.maxY) / 20)
                                                     : (domain.maxY - domain.minY) / 20
            axis.min = Qt.binding(function() { return axisValue(domain.minY - offset, axis.format !== undefined) })
            axis.max = Qt.binding(function() { return axisValue(domain.maxY + offset, axis.format !== undefined) })
        }
        else {
            offset = domain.minX === domain.maxX ? (domain.maxX === 0 ? 0.1 : Math.abs(domain.maxX) / 20)
                                                 : 0//(domain.maxX - domain.minX) / 20
            axis.min = Qt.binding(function() { return axisValue(domain.minX - offset, axis.format !== undefined) })
            axis.max = Qt.binding(function() { return axisValue(domain.maxX + offset, axis.format !== undefined) })
        }
    }

    function setAxesDomain(index, axis_x, axis_y) {
        if(axis_x === null || axis_y === null)
            return

        var domain = seriesListMapper.domain()
        var currentDomain = seriesListMapper.domainAt(index)

        switch(chartAxesType) {
        case ChartViewExtension.OneAxis:
            if(index === 0) {
                updateAxisDomain(axis_y, domain, Qt.Vertical)
                updateAxisDomain(axis_x, domain, Qt.Horizontal)
            }
            break
        case ChartViewExtension.MultipleHorizontalAxes:
            if(index === 0) // вертикальная ось только одна
                updateAxisDomain(axis_y, domain, Qt.Vertical)
            updateAxisDomain(axis_x, currentDomain, Qt.Horizontal)
            break
        case ChartViewExtension.MultipleVerticalAxes:
            if(index === 0) // горизонтальная ось только одна
                updateAxisDomain(axis_x, domain, Qt.Horizontal)
            updateAxisDomain(axis_y, currentDomain, Qt.Vertical)
            break
        case ChartViewExtension.MultipleAxes:
            // Все оси отличаются
            updateAxisDomain(axis_x, currentDomain, Qt.Horizontal)
            updateAxisDomain(axis_y, currentDomain, Qt.Vertical)
            break
        default: break
        }
    }
}

