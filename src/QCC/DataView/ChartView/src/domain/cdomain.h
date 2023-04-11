#ifndef CDOMAIN_H
#define CDOMAIN_H

#include <QObject>

class CDomain : public QObject
{
    Q_OBJECT

public:
    explicit CDomain(QObject *parent = nullptr);
    virtual ~CDomain() {}

    virtual void setRange(qreal minX, qreal maxX, qreal minY, qreal maxY) = 0;

    virtual void move(qreal dx, qreal dy) = 0;
    virtual void setScale(qreal sx, qreal sy) = 0;
    virtual void zoomIn() = 0;
    virtual void zoomOut() = 0;
    void zoom(qreal minX, qreal maxX, qreal minY, qreal maxY);

    void resetTransform();

    void setRangeX(qreal min, qreal max);
    void setRangeY(qreal min, qreal max);

    void setScaleX(const qreal &sx);
    void setScaleY(const qreal &sy);

    qreal getScaleX() const;
    qreal getScaleY() const;

    qreal getMinX() const;
    qreal getMaxX() const;
    qreal getMaxY() const;
    qreal getMinY() const;

    void setMinX(const qreal &min);
    void setMaxX(const qreal &max);
    void setMinY(const qreal &min);
    void setMaxY(const qreal &max);

    void setTransformResetMinX(const qreal &min);
    void setTransformResetMaxX(const qreal &max);
    void setTransformResetMinY(const qreal &min);
    void setTransformResetMaxY(const qreal &max);

    bool isMoved() const;
    bool isZoomed() const;


protected:
    qreal minX;
    qreal maxX;
    qreal minY;
    qreal maxY;

    qreal transformResetMinX;
    qreal transformResetMaxX;
    qreal transformResetMinY;
    qreal transformResetMaxY;

    bool ismoved;
    bool iszoomed;
    qreal scaleX;
    qreal scaleY;

signals:
    void updated();
    void rangeHorizontalChanged();
    void rangeVerticalChanged();
};

#endif // CDOMAIN_H
