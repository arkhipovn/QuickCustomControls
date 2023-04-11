#ifndef LINENODE_H
#define LINENODE_H

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGOpaqueTextureMaterial>
#include <QSGVertexColorMaterial>
#include "../domain/cdomain.h"
#include <qmath.h>

class LineNode : public QSGGeometryNode
{
public:
    LineNode(float w = 2, const QColor &color = QColor(Qt::black));
    ~LineNode();

    // bound - область на которой рисовать
    // domain - min, max
    void updateLog(const QRectF &bounds, const CDomain *domain, const QList <QPointF> &samples, int base);
    void update(const QRectF &bounds, const CDomain *domain, const QList <QPointF> &samples);
    void setColor(const QColor &color);
    void setLineWidth(float w);
};

#endif // LINENODE_H
