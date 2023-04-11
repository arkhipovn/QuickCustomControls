#include "cxydomain.h"

CXYDomain::CXYDomain(QObject *parent) : CDomain(parent)
{

}

void CXYDomain::setRange(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    bool axisXChanged = false;
    bool axisYChanged = false;

    if(!qFuzzyCompare(this->minX, minX) || !qFuzzyCompare(this->maxX, maxX)) {
        this->minX = minX;
        this->maxX = maxX;
        axisXChanged = true;
        emit rangeHorizontalChanged();
    }

    if(!qFuzzyCompare(this->minY, minY) || !qFuzzyCompare(this->maxY, maxY)) {
        this->minY = minY;
        this->maxY = maxY;
        axisYChanged = true;
        emit rangeVerticalChanged();
    }

    if(!iszoomed && !ismoved) {
        // если не двигали до этого и не увеличивали
        if(axisXChanged) {
            this->transformResetMinX = this->minX;
            this->transformResetMaxX = this->maxX;
        }

        if(axisYChanged) {
            this->transformResetMinY = this->minY;
            this->transformResetMaxY = this->maxY;
        }
    }

    if(axisXChanged || axisYChanged) {
        emit updated();
    }
}

void CXYDomain::move(qreal dx, qreal dy)
{
    if(!qFuzzyCompare(dx + 1, 1.) || !qFuzzyCompare(dy + 1, 1.)) {
        ismoved = true;
        setRange(minX - dx, maxX - dx, minY + dy, maxY + dy);
    }
}

void CXYDomain::zoomIn()
{
    iszoomed = true;

    //    qreal nscaleX = CXYDomain::midZoom(scaleX + ZOOM_ACCURACY);
    //    qreal nscaleY = CXYDomain::midZoom(scaleY + ZOOM_ACCURACY);

    qreal nminX = minX + (maxX - minX) * 0.1;
    qreal nmaxX = maxX - (maxX - minX) * 0.1;
    qreal nminY = minY + (maxY - minY) * 0.1;
    qreal nmaxY = maxY - (maxY - minY) * 0.1;

    //    qreal nminX = minX + (transformResetMaxX - transformResetMinX) * (nscaleX - scaleX);
    //    qreal nmaxX = maxX - (transformResetMaxX - transformResetMinX) * (nscaleX - scaleX);
    //    qreal nminY = minY + (transformResetMaxY - transformResetMinY) * (nscaleY - scaleY);
    //    qreal nmaxY = maxY - (transformResetMaxY - transformResetMinY) * (nscaleY - scaleY);

    if(nminX > nmaxX || nminY > nmaxY) return;

    //    scaleX = nscaleX;
    //    scaleY = nscaleY;

    setRange(nminX, nmaxX, nminY, nmaxY);
}

void CXYDomain::zoomOut()
{
    iszoomed = true;

    //    qreal nscaleX = CXYDomain::midZoom(scaleX - ZOOM_ACCURACY);
    //    qreal nscaleY = CXYDomain::midZoom(scaleY - ZOOM_ACCURACY);

    qreal nminX = minX - (maxX - minX) * 0.1;
    qreal nmaxX = maxX + (maxX - minX) * 0.1;
    qreal nminY = minY - (maxY - minY) * 0.1;
    qreal nmaxY = maxY + (maxY - minY) * 0.1;

    //    qreal nminX = minX + (transformResetMaxX - transformResetMinX) * (nscaleX - scaleX);
    //    qreal nmaxX = maxX - (transformResetMaxX - transformResetMinX) * (nscaleX - scaleX);
    //    qreal nminY = minY + (transformResetMaxY - transformResetMinY) * (nscaleY - scaleY);
    //    qreal nmaxY = maxY - (transformResetMaxY - transformResetMinY) * (nscaleY - scaleY);

    if(nminX > nmaxX || nminY > nmaxY) return;

    //    scaleX = nscaleX;
    //    scaleY = nscaleY;

    setRange(nminX, nmaxX, nminY, nmaxY);
}

void CXYDomain::setScale(qreal sx, qreal sy)
{
    iszoomed = true;

    qreal nscaleX = CXYDomain::midZoom(sx);
    qreal nscaleY = CXYDomain::midZoom(sy);

    qreal nminX = minX + (transformResetMaxX - transformResetMinX) * (nscaleX - scaleX);
    qreal nmaxX = maxX - (transformResetMaxX - transformResetMinX) * (nscaleX - scaleX);
    qreal nminY = minY + (transformResetMaxY - transformResetMinY) * (nscaleY - scaleY);
    qreal nmaxY = maxY - (transformResetMaxY - transformResetMinY) * (nscaleY - scaleY);

    if(nminX > nmaxX || nminY > nmaxY) return;

    scaleX = nscaleX;
    scaleY = nscaleY;

    setRange(nminX, nmaxX, nminY, nmaxY);
}

qreal CXYDomain::midZoom(qreal value)
{
    if(value < MIN_SCALE) return MIN_SCALE;
    if(value > MAX_SCALE) return MAX_SCALE;
    return value;
}
