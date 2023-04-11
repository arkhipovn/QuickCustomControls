#include "caxis.h"

#define DOUBLE_LIMITS 1.e-19

CAxis::CAxis(TypeAxis type, QQuickItem *parent) :
    QQuickPaintedItem(parent), type(type)
{
    setFlag(ItemHasContents, true); //@N

    domain = nullptr;

    tickCountX = 4;
    tickCountY = 5;

    gridPen = QPen(Qt::DotLine);
    gridPen.setColor(QColor("#A0A0A0"));    //E1E1E1

    gridSolidPen = QPen(Qt::SolidLine);
    gridSolidPen.setColor(gridPen.color().darker(105));

    axisPen = QPen(Qt::SolidLine);
    axisPen.setColor(Qt::black);

    labelsPen = QPen(Qt::SolidLine);
    labelsPen.setColor(Qt::black);
}

void CAxis::setTickCountX(int count)
{
    if(tickCountX != count) {
        tickCountX = count;
        emit tickCountXChanged();
        update();
    }
}

void CAxis::setTickCountY(int count)
{
    if(tickCountY != count) {
        tickCountY = count;
        emit tickCountYChanged();
        update();
    }
}

void CAxis::setGridColor(const QColor &c)
{
    if(gridPen.color() != c) {
        gridPen.setColor(c);
        gridSolidPen.setColor(c.darker(105));
        emit gridColorChanged();
        update();
    }
}

void CAxis::setAxisColor(const QColor &c)
{
    if(axisPen.color() != c) {
        axisPen.setColor(c);
        emit axisColorChanged();
        update();
    }
}

void CAxis::setLabelsColor(const QColor &c)
{
    if(labelsPen.color() != c) {
        labelsPen.setColor(c);
        emit labelsColorChanged();
        update();
    }
}

void CAxis::setPlotArea(const QRectF &rect)
{
    if(rect != plotArea) {
        plotArea = rect;
        emit plotAreaChanged();
    }
}

void CAxis::setLabelsFont(const QFont &font)
{
    if(labelsFont != font) {
        labelsFont = font;
        update();
    }
}

QColor CAxis::getGridColor() const { return gridPen.color(); }
QColor CAxis::getAxisColor() const { return axisPen.color(); }
QColor CAxis::getLabelsColor() const { return labelsPen.color(); }
int CAxis::getTickCountX() const { return tickCountX; }
int CAxis::getTickCountY() const { return tickCountY; }
QRectF CAxis::getPlotArea() const { return plotArea; }
CDomain *CAxis::getDomain() const { return domain; }

qreal CAxis::getMinX() const { return domain->getMinX(); }
qreal CAxis::getMaxX() const { return domain->getMaxX(); }
qreal CAxis::getMinY() const { return domain->getMinY(); }
qreal CAxis::getMaxY() const { return domain->getMaxY(); }

void CAxis::setMinX(const qreal &min)
{
    domain->setMinX(min);
}

void CAxis::setMaxX(const qreal &max)
{
    domain->setMaxX(max);
}

void CAxis::setMinY(const qreal &min)
{
    domain->setMinY(min);
}

void CAxis::setMaxY(const qreal &max)
{
    domain->setMaxY(max);
}

CAxis::TypeAxis CAxis::getType() const
{
    return type;
}

QRect CAxis::getFontMetrix(const QFont &font, const QString &text)
{
    QFontMetrics fm(font);
    return fm.boundingRect(text);
}


