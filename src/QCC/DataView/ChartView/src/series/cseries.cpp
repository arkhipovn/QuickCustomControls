#include "cseries.h"

CSeries::CSeries(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    setClip(true);

    domain = nullptr;
    axis = nullptr;
}

void CSeries::setAxis(CAxis *axis)
{
    if(axis == nullptr) return;
    if(this->axis != axis) {
        this->axis = axis;
        QRectF plotArea = axis->getPlotArea();
        setPosition(QPointF(plotArea.x(), plotArea.y()));
        setSize(QSizeF(plotArea.width(), plotArea.height()));

        QObject::connect(axis->getDomain(), &CDomain::updated, this, &CSeries::update);
        QObject::connect(axis, &CAxis::plotAreaChanged, this, &CSeries::setPlotArea);
        emit axisChanged();
        //                                update(); //@N
    }
}

CAxis *CSeries::getAxis() const { return axis; }
qreal CSeries::getMinX() const { return domain->getMinX(); }
qreal CSeries::getMaxX() const { return domain->getMaxX(); }
qreal CSeries::getMinY() const { return domain->getMinY(); }
qreal CSeries::getMaxY() const { return domain->getMaxY(); }

void CSeries::setPlotArea()
{
    if(axis == nullptr) return;
    QRectF plotArea = axis->getPlotArea();
    setX(plotArea.x());
    setY(plotArea.y());
    setWidth(plotArea.width());
    setHeight(plotArea.height());
    update();
}

QString CSeries::getLabel() const
{
    return label;
}

void CSeries::setLabel(const QString &label)
{
    if(this->label != label) {
        this->label = label;
        emit labelChanged();
    }
}

