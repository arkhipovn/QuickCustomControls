#ifndef CLINESERIES_H
#define CLINESERIES_H

#include <QVariantList>
#include "../axis/clogvalueaxis.h"

#include "linenode.h"
#include "cseries.h"
#include "../domain/cxydomain.h"

class CLineSeries : public CSeries
{
    Q_OBJECT

    Q_PROPERTY(int size READ getSize NOTIFY samplesChanged)
    Q_PROPERTY(QList <QPointF> samples READ getSamples WRITE setSamples NOTIFY samplesChanged)

    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float lineWidth READ getLineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

public:
    explicit CLineSeries(QQuickItem *parent = 0);

    QList <QPointF> getSamples() const;

    Q_INVOKABLE void setSamples(const QVector <QPointF> &samples);
    Q_INVOKABLE void setSamples(const QList <QPointF> &samples);
    Q_INVOKABLE void appendSamples(const QList <QPointF> &samples);

    void setLineWidth(float w);
    QColor getColor() const;

    void setColor(const QColor &c);
    float getLineWidth() const;

    int getSize() const;

public slots:
    void clear();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    QList <QPointF> samples;

    QColor color;
    float lineWidth;

signals:
    void samplesChanged();

    void lineWidthChanged();
    void colorChanged();
};

#endif // CLINESERIES_H

