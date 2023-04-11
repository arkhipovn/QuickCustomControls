#ifndef CSERIES_H
#define CSERIES_H

#include <QQuickItem>
#include "../axis/caxis.h"

class CSeries : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)

    Q_PROPERTY(CAxis* axis READ getAxis WRITE setAxis NOTIFY axisChanged)

    Q_PROPERTY(qreal maxY READ getMaxY NOTIFY rangeVerticalChanged)
    Q_PROPERTY(qreal minY READ getMinY NOTIFY rangeVerticalChanged)
    Q_PROPERTY(qreal minX READ getMinX NOTIFY rangeHorizontalChanged)
    Q_PROPERTY(qreal maxX READ getMaxX NOTIFY rangeHorizontalChanged)

public:
    explicit CSeries(QQuickItem *parent = nullptr);
    virtual ~CSeries() {}

    void setAxis(CAxis* axis);
    CAxis *getAxis() const;

    qreal getMinX() const;
    qreal getMaxX() const;
    qreal getMaxY() const;
    qreal getMinY() const;

    enum TypeSeries {
        LineSeries
    };
    Q_ENUMS(TypeSeries)

    QString getLabel() const;

public slots:
    void setLabel(const QString &label);
    virtual void clear() {}//= 0;

private slots:
    void setPlotArea();

protected:
    TypeSeries type;
    CAxis *axis;
    CDomain *domain;
    QString label;

signals:
    void axisChanged();
    void rangeHorizontalChanged();
    void rangeVerticalChanged();
    void labelChanged();
};

#endif // CSERIES_H

