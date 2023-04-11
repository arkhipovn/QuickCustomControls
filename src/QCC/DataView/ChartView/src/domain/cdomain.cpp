#include "cdomain.h"

CDomain::CDomain(QObject *parent) : QObject(parent)
{
    iszoomed = false;
    ismoved = false;
    scaleX = 1;
    scaleY = 1;

    minX = 1; maxX = 10;
    minY = 1; maxY = 10;

    transformResetMinX = 0; transformResetMaxX = 1;
    transformResetMinY = 0; transformResetMaxY = 1;
}

void CDomain::zoom(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    iszoomed = true;
    setRange(minX, maxX, minY, maxY);
}

void CDomain::resetTransform()
{
    scaleX = 1; scaleY = 1;
    ismoved = false;
    iszoomed = false;
    setRange(transformResetMinX, transformResetMaxX, transformResetMinY, transformResetMaxY);
}

void CDomain::setRangeX(qreal min, qreal max)
{
    setRange(min, max, minY, maxY);
}

void CDomain::setRangeY(qreal min, qreal max)
{
    setRange(minX, maxX, min, max);
}

void CDomain::setMinX(const qreal &min)
{
    setRange(min, maxX, minY, maxY);
}

void CDomain::setMaxX(const qreal &max)
{
    setRange(minX, max, minY, maxY);
}

void CDomain::setMinY(const qreal &min)
{
    setRange(minX, maxX, min, maxY);
}

void CDomain::setMaxY(const qreal &max)
{
    setRange(minX, maxX, minY, max);
}

void CDomain::setTransformResetMinX(const qreal &min)
{
    transformResetMinX = min;
}

void CDomain::setTransformResetMaxX(const qreal &max)
{
    transformResetMaxX = max;
}

void CDomain::setTransformResetMinY(const qreal &min)
{
    transformResetMinY = min;
}

void CDomain::setTransformResetMaxY(const qreal &max)
{
    transformResetMaxY = max;
}

bool CDomain::isMoved() const
{
    return ismoved;
}

bool CDomain::isZoomed() const
{
    return iszoomed;
}

void CDomain::setScaleX(const qreal &sx)
{
    setScale(sx, scaleY);
}

void CDomain::setScaleY(const qreal &sy)
{
    setScale(scaleX, sy);
}

qreal CDomain::getScaleX() const { return scaleX; }
qreal CDomain::getScaleY() const { return scaleY; }
qreal CDomain::getMinX() const { return minX; }
qreal CDomain::getMaxX() const { return maxX; }
qreal CDomain::getMinY() const { return minY; }
qreal CDomain::getMaxY() const { return maxY; }
