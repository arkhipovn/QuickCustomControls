#ifndef CAXIS_H
#define CAXIS_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QFontMetrics>
#include <QRectF>
#include "../domain/cdomain.h"

#define DASH_SIZE 4

class CAxis : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(int type READ getType CONSTANT)

    Q_PROPERTY(QColor labelsColor READ getLabelsColor WRITE setLabelsColor NOTIFY labelsColorChanged)
    Q_PROPERTY(QColor axisColor READ getAxisColor WRITE setAxisColor NOTIFY axisColorChanged)
    Q_PROPERTY(QColor gridColor READ getGridColor WRITE setGridColor NOTIFY gridColorChanged)

    Q_PROPERTY(int tickCountX READ getTickCountX WRITE setTickCountX NOTIFY tickCountXChanged)
    Q_PROPERTY(int tickCountY READ getTickCountY WRITE setTickCountY NOTIFY tickCountYChanged)

    Q_PROPERTY(QRectF plotArea READ getPlotArea NOTIFY plotAreaChanged)

    Q_PROPERTY(qreal maxY WRITE setMaxY READ getMaxY NOTIFY rangeVerticalChanged)
    Q_PROPERTY(qreal minY WRITE setMinY READ getMinY NOTIFY rangeVerticalChanged)
    Q_PROPERTY(qreal minX WRITE setMinX READ getMinX NOTIFY rangeHorizontalChanged)
    Q_PROPERTY(qreal maxX WRITE setMaxX READ getMaxX NOTIFY rangeHorizontalChanged)

public:
    enum TypeAxis {
        ValueAxisType,
        DateTimeAxisType,
        LogValueAxisType,
    };
    Q_ENUMS(TypeAxis)

    explicit CAxis(TypeAxis type, QQuickItem *parent = 0);
    virtual ~CAxis() = default;


    static QRect getFontMetrix(const QFont &font, const QString &text);

    void setLabelsFont(const QFont &font);

    void setGridColor(const QColor &c);
    void setAxisColor(const QColor &c);
    void setLabelsColor(const QColor &c);

    QColor getGridColor() const;
    QColor getAxisColor() const;
    QColor getLabelsColor() const;

    void setTickCountX(int count);
    void setTickCountY(int count);

    int getTickCountX() const;
    int getTickCountY() const;

    QRectF getPlotArea() const;

    CDomain *getDomain() const;

    qreal getMinX() const;
    qreal getMaxX() const;
    qreal getMaxY() const;
    qreal getMinY() const;

    virtual void setMinX(const qreal &min);
    virtual void setMaxX(const qreal &max);
    virtual void setMinY(const qreal &min);
    virtual void setMaxY(const qreal &max);

    virtual QPointF mapToPosition(const QPointF &value) = 0;
    virtual QPointF mapToValue(const QPointF &point) = 0;

    TypeAxis getType() const;

protected:
    TypeAxis type;

    QPen labelsPen;
    QPen gridPen;
    QPen gridSolidPen;
    QPen axisPen;

    QFont labelsFont;

    int tickCountX;     // сетка по оси X
    int tickCountY;     // сетка по оси Y

    QRectF plotArea;

    CDomain *domain;

    void setPlotArea(const QRectF &rect);

signals:
    void labelsColorChanged();
    void axisColorChanged();
    void gridColorChanged();

    void tickCountXChanged();
    void tickCountYChanged();

    void plotAreaChanged();
    void axisRangeChanged();

    void rangeHorizontalChanged();
    void rangeVerticalChanged();
};

#endif // CAXIS_H

