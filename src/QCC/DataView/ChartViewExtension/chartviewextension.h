#ifndef CHARTVIEWEXTENSION_H
#define CHARTVIEWEXTENSION_H

#include <QObject>
#include <QAbstractSeries>

QT_CHARTS_BEGIN_NAMESPACE

namespace ChartViewExtension {
    Q_NAMESPACE

    enum ChartAxes {
        OneAxis,
        MultipleHorizontalAxes,
        MultipleVerticalAxes,
        MultipleAxes
    };
    Q_ENUMS(ChartAxes)

    enum OpenGlUsage {
        Use,
        NotUse,
        Auto
    };
    Q_ENUMS(OpenGlUsage)

    enum XYSeriesType {
        SeriesTypeLine = QAbstractSeries::SeriesTypeLine,
        SeriesTypeSpline = QAbstractSeries::SeriesTypeSpline,
        SeriesTypeScatter = QAbstractSeries::SeriesTypeScatter
    };
    Q_ENUMS(XYSeriesType)

    enum ChartHandlerMode {
        Marker = 0,
        Zooming,
        Inaction
    };
    Q_ENUMS(ChartHandlerMode)

}

QT_CHARTS_END_NAMESPACE

#endif // CHARTVIEWEXTENSION_H
