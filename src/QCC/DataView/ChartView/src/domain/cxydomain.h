#ifndef CXYDOMAIN_H
#define CXYDOMAIN_H

#include "cdomain.h"

#define ZOOM_ACCURACY 0.05
#define MIN_SCALE 0.05
#define MAX_SCALE 100

class CXYDomain : public CDomain
{
    Q_OBJECT

public:
    explicit CXYDomain(QObject *parent = nullptr);

    void setRange(qreal minX, qreal maxX, qreal minY, qreal maxY);

    void move(qreal dx, qreal dy);
    void zoomIn();
    void zoomOut();
    void setScale(qreal sx, qreal sy);

    static qreal midZoom(qreal value);
};

#endif // CXYDOMAIN_H
