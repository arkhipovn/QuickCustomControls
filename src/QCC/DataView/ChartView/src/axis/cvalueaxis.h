#ifndef CVALUEAXIS_H
#define CVALUEAXIS_H

#include "caxis.h"
#include "qmath.h"
#include "../domain/cxydomain.h"
#include <QDebug>

class CValueAxis : public CAxis
{
    Q_OBJECT

    Q_PROPERTY(bool offsetActive READ getOffsetActive WRITE setOffsetActive NOTIFY offsetActiveChanged)
    Q_PROPERTY(bool factorActive READ getFactorActive WRITE setFactorActive NOTIFY factorActiveChanged)

public:
    explicit CValueAxis(QQuickItem *parent = 0);
    explicit CValueAxis(TypeAxis type, QQuickItem *parent = 0);
    virtual ~CValueAxis() = default;

    QPointF mapToPosition(const QPointF &value) override;
    QPointF mapToValue(const QPointF &point) override;

    bool getOffsetActive() const;
    void setOffsetActive(bool value);

    bool getFactorActive() const;
    void setFactorActive(bool value);

protected:
    void paint(QPainter *painter);

private slots:
    void updateAxisSettings();

protected:
    // например. Если Разница между макс и мин будет небольшая (0.00302098 - 0.00302086), то offset это 0.0030208, а factor = 1e-7
    qreal offset;
    qreal factor; // коэф на что нужно умножать оси..

    QPointF interval;   // интервал между точками
    int precision;      // точность значений по оси Y

    bool offsetActive;  // нужно ли применять offset
    bool factorActive;  // нужно ли добавлять коэф по оси х

signals:
    void offsetActiveChanged();
    void factorActiveChanged();
};


#endif // CVALUEAXIS_H
