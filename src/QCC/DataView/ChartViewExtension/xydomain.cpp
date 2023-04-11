#include "xydomain.h"
#include <QDebug>

XYDomain::XYDomain(QObject *parent) : QObject(parent), minX_(1), maxX_(2), minY_(1), maxY_(2)
{

}

void XYDomain::setRange(Qt::Orientation orientation, qreal min, qreal max)
{
    if(orientation == Qt::Vertical)
        setRangeY(min, max);
    else
        setRangeX(min, max);
}

void XYDomain::setRangeHorizontal(qreal min, qreal max)
{
    setRangeX(min, max);
}

void XYDomain::setRangeVertical(qreal min, qreal max)
{
    setRangeY(min, max);
}

void XYDomain::setRangeX(qreal min, qreal max)
{
    if(qFuzzyCompare(minX_, min) && qFuzzyCompare(maxX_, max))
        return;

    minX_ = min;
    maxX_ = max;
    emit rangeHorizontalChanged();
    emit updated();
}

void XYDomain::setRangeY(qreal min, qreal max)
{
    if(qFuzzyCompare(minY_, min) && qFuzzyCompare(maxY_, max))
        return;

    minY_ = min;
    maxY_ = max;
    emit rangeVerticalChanged();
    emit updated();
}

void XYDomain::setRange(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    bool axisXChanged = false;
    bool axisYChanged = false;

    if(!qFuzzyCompare(minX_, minX) || !qFuzzyCompare(maxX_, maxX)) {
        minX_ = minX;
        maxX_ = maxX;
        axisXChanged = true;
        emit rangeHorizontalChanged();
    }

    if(!qFuzzyCompare(minY_, minY) || !qFuzzyCompare(maxY_, maxY)) {
        minY_ = minY;
        maxY_ = maxY;
        axisYChanged = true;
        emit rangeVerticalChanged();
    }

    if(axisXChanged || axisYChanged) {
        emit updated();
    }
}

qreal XYDomain::minX() const
{
    return minX_;
}

void XYDomain::setMinX(const qreal &minX)
{
    if(qFuzzyCompare(minX_, minX))
        return;
    minX_ = minX;
    emit rangeHorizontalChanged();
    emit updated();
}

qreal XYDomain::maxX() const
{
    return maxX_;
}

void XYDomain::setMaxX(const qreal &maxX)
{
    if(qFuzzyCompare(maxX_, maxX))
        return;

    maxX_ = maxX;
    emit rangeHorizontalChanged();
    emit updated();
}

qreal XYDomain::minY() const
{
    return minY_;
}

void XYDomain::setMinY(const qreal &minY)
{
    if(qFuzzyCompare(minY_, minY))
        return;

    minY_ = minY;
    emit rangeVerticalChanged();
    emit updated();
}

qreal XYDomain::maxY() const
{
    return maxY_;
}

void XYDomain::setMaxY(const qreal &maxY)
{
    if(qFuzzyCompare(maxY_, maxY))
        return;

    maxY_ = maxY;
    emit rangeVerticalChanged();
    emit updated();
}

void XYDomain::reset()
{
    setRange(1, 2, 1, 2);
}

