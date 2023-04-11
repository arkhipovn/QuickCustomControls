#ifndef CLOGVALUEAXIS_H
#define CLOGVALUEAXIS_H

#include "caxis.h"
#include "qmath.h"
#include "../domain/cxydomain.h"
#include <QDebug>

class CLogValueAxis : public CAxis
{
    Q_OBJECT

public:
    explicit CLogValueAxis(QQuickItem *parent = 0);

    int getBase() const;

    QPointF mapToPosition(const QPointF &value) override;
    QPointF mapToValue(const QPointF &point) override;


    void setMinX(const qreal &min) override;
    void setMaxX(const qreal &max) override;
    void setMinY(const qreal &min) override;
    void setMaxY(const qreal &max) override;


protected:
    void paint(QPainter *painter);

private:
    int base;

    QPointF lnAxisX;
    QPointF lnAxisY;

    void calcAxisSettings();
    QString toStrValue(qreal value, QRect &bound);
};

#endif // CLOGVALUEAXIS_H
