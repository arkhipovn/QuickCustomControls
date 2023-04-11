#ifndef CCHARTVIEW_H
#define CCHARTVIEW_H

#include <QQuickWindow>
#include <QQuickItem>
#include <QRect>
#include <QDebug>

#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>

#include "qmath.h"

#include "series/clineseries.h"
#include "axis/caxis.h"

class CChartView : public QQuickItem
{
    Q_OBJECT

    Q_ENUMS(CSeries::TypeSeries)
    Q_ENUMS(CAxis::TypeAxis)

    Q_PROPERTY(CAxis *axis READ getAxis WRITE setAxis NOTIFY axisChanged)
    Q_PROPERTY(QQmlListProperty<CSeries> listSeries READ getListSeries NOTIFY seriesChanged)
    Q_PROPERTY(int count READ count NOTIFY seriesChanged)
    Q_PROPERTY(QFont labelsFont READ getLabelsFont WRITE setLabelsFont NOTIFY labelsFontChanged)

public:
    explicit CChartView(QQuickItem *parent = nullptr);

    void setLabelsFont(const QFont &font);
    QFont getLabelsFont() const;

    QQmlListProperty<CSeries> getListSeries();

    void setAxis(CAxis *axis);
    CAxis *getAxis() const;

public slots:
    //    CAxis createAxis() const;
    CSeries *createSeries(int /*CSeries::TypeSeries*/ type, const QString &label = "", const QString &color = "");
    CSeries *getSeries(int ind) const;
    void removeSeries();
    void removeSeries(int ind);

    QPointF mapToPosition(const QPointF &value);
    QPointF mapToValue(const QPointF &point);
    void clear();

    int count();


    // если true, то значит не смотрим на то, что visible = false
    void forceAxisRange(bool init = false);

    void saveChart(const QString &fileName, const QImage &img);
    void printChart(const QImage &img, const QString &title = "");

protected:
    void componentComplete();
    void geometryChanged(const QRectF &newGeometry,
                         const QRectF &oldGeometry);

private slots:
    void childrenChanging();     //@N мб есть уже слот?

signals:
    void labelsFontChanged();
    void seriesChanged();
    void axisChanged();

private:
    QList <CSeries*> series;
    CAxis *axis;

    QFont labelsFont;

    static int countData(QQmlListProperty <CSeries> *list);
    static CSeries *atData(QQmlListProperty <CSeries> *list, int index);
};

#endif // CCHARTVIEW_H



