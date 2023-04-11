#include "linenode.h"

LineNode::LineNode(float w, const QColor &color)
{
    QSGGeometry *g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
    g->setLineWidth(w);
    g->setDrawingMode(GL_LINE_STRIP);

    setGeometry(g);
    setFlag(QSGNode::OwnsGeometry);

    QSGFlatColorMaterial *m = new QSGFlatColorMaterial;
    m->setColor(color);

    setMaterial(m);
    setFlag(QSGNode::OwnsMaterial);
}

LineNode::~LineNode()
{

}

void LineNode::setColor(const QColor &color)
{
    QSGFlatColorMaterial *m = static_cast <QSGFlatColorMaterial *> (material());
    m->setColor(color);
}

void LineNode::setLineWidth(float w)
{
    QSGGeometry *g = geometry();
    g->setLineWidth(w);
}

void LineNode::updateLog(const QRectF &bounds, const CDomain *domain, const QList<QPointF> &samples, int base)
{
    QSGGeometry *g = geometry();

    if(samples.isEmpty()) g->allocate(1); //@N
    else g->allocate(samples.size());

    if(!samples.isEmpty()) {
        qreal w = bounds.width();
        qreal h = bounds.height();

        QSGGeometry::Point2D *vertexes = g->vertexDataAsPoint2D();

        const qreal logMaxX = qLn(domain->getMaxX()) / qLn(base);
        const qreal logMinX = qLn(domain->getMinX()) / qLn(base);
        const qreal logMaxY = qLn(domain->getMaxY()) / qLn(base);
        const qreal logMinY = qLn(domain->getMinY()) / qLn(base);

        for (int i = 0; i < samples.size(); ++i) {
            const qreal valueX = qLn(samples.at(i).x()) / qLn(base);
            const qreal valueY = qLn(samples.at(i).y()) / qLn(base);

            qreal vx = (valueX - logMinX) / (logMaxX - logMinX);
            qreal vy = (valueY - logMinY) / (logMaxY - logMinY);

            float x = bounds.x() + vx * w;
            float y = bounds.y() + (1 - vy) * h;
            vertexes[i].set(x, y);
        }
    }
    markDirty(QSGNode::DirtyGeometry);
}

void LineNode::update(const QRectF &bounds, const CDomain *domain, const QList<QPointF> &samples)
{
    QSGGeometry *g = geometry();

    if(samples.isEmpty()) g->allocate(1); //@N
    else g->allocate(samples.size());

    if(!samples.isEmpty()) {
        qreal w = bounds.width();
        qreal h = bounds.height();

        QSGGeometry::Point2D *vertexes = g->vertexDataAsPoint2D();

        for (int i = 0; i < samples.size(); ++i) {
            qreal vx = (samples.at(i).x() - domain->getMinX()) / (domain->getMaxX() - domain->getMinX());
            qreal vy = (samples.at(i).y() - domain->getMinY()) / (domain->getMaxY() - domain->getMinY());

            float x = bounds.x() + vx * w;
            float y = bounds.y() + (1 - vy) * h;
            vertexes[i].set(x, y);
        }
    }
    markDirty(QSGNode::DirtyGeometry);
}
