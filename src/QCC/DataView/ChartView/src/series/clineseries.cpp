#include "clineseries.h"
#include <QTimer>

CLineSeries::CLineSeries(QQuickItem *parent):
    CSeries(parent)
{
    type = LineSeries;
    domain = new CXYDomain(this);
    QObject::connect(domain, &CDomain::rangeVerticalChanged, this, &CSeries::rangeVerticalChanged);
    QObject::connect(domain, &CDomain::rangeHorizontalChanged, this, &CSeries::rangeHorizontalChanged);

    lineWidth = 2;
    color = QColor("#24ACFF");
}

void CLineSeries::setSamples(const QVector<QPointF> &samples)
{
    setSamples(samples.toList());
}

void CLineSeries::setSamples(const QList <QPointF> &samples)
{
    this->samples = samples;
    if(samples.isEmpty()) {
        domain->setRange(1, 1, 1, 1);
    }
    else {
        qreal minX = samples.first().x(); qreal maxX = samples.first().x();
        qreal minY = samples.first().y(); qreal maxY = samples.first().y();
        for(int i = 0; i < samples.size(); ++i) {
            if(minY > samples.at(i).y()) minY = samples.at(i).y();
            if(maxY < samples.at(i).y()) maxY = samples.at(i).y();

            if(minX > samples.at(i).x()) minX = samples.at(i).x();
            if(maxX < samples.at(i).x()) maxX = samples.at(i).x();
        }

        domain->setRange(minX, maxX, minY, maxY);
    }
    emit samplesChanged();
    update();
}

void CLineSeries::appendSamples(const QList <QPointF> &samples)
{
    setSamples(this->samples + samples);
}

QSGNode *CLineSeries::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QRectF bounds = boundingRect();
    LineNode *n = static_cast <LineNode *> (oldNode);

    if(!n) {
        n = new LineNode(lineWidth, color);
    }
    else {
        n->setLineWidth(lineWidth);
        n->setColor(color);
    }
    CDomain *d = (axis == nullptr) ? domain : axis->getDomain();

    auto s = qobject_cast<CLogValueAxis *>(axis);
    if(s) n->updateLog(bounds, d, samples, s->getBase()); // логарифмический масштаб
    else n->update(bounds, d, samples);
    return n;
}

void CLineSeries::setLineWidth(float w)
{
    if(lineWidth != w) {
        lineWidth = w;
        emit lineWidthChanged();
        update();
    }
}

void CLineSeries::setColor(const QColor &c)
{
    if(color != c) {
        color = c;
        emit colorChanged();
        update();
    }
}

QList <QPointF> CLineSeries::getSamples() const  { return samples; }

QColor CLineSeries::getColor() const { return color; }
float CLineSeries::getLineWidth() const { return lineWidth; }

int CLineSeries::getSize() const { return samples.size(); }

void CLineSeries::clear()
{
    this->samples.clear();
    domain->setRange(1, 10, 1, 10);
}

