#include "arrowitem.h"
#include <QtMath>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QQuickWindow>
#include <QRectF>
#include <QDebug>

ArrowItem::ArrowItem(QQuickItem *parent) : QQuickItem(parent),
    lineColor_(Qt::black),
    arrowColor_(Qt::green),
    lineWidth_(1),
    indicatorForm_(IndicatorForm::Arrow)
{
    setFlag(ItemHasContents, true);
}

qreal ArrowItem::lineWidth() const
{
    return lineWidth_;
}

void ArrowItem::setLineWidth(const qreal &lineWidth)
{
    if(this->lineWidth_ != lineWidth) {
        this->lineWidth_ = lineWidth;
        emit lineWidthChanged();
        update();
    }
}

QSGNode *ArrowItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData)
    QSGGeometryNode *node = static_cast <QSGGeometryNode *> (oldNode);
    QRectF rect = boundingRect();

    // Если при обновлении нода не существует, то необходимо создать все объекты и прикрепить их к ноде
    if (!node) {
        node = new QSGGeometryNode();
        QSGGeometry *g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);

#if QT_VERSION < QT_VERSION_CHECK(5, 8, 0)
        g->setDrawingMode(GL_LINES);        //@N проверить..
#else
        g->setDrawingMode(QSGGeometry::DrawLines);
#endif
        g->setLineWidth(realLineWidth());

        node->setGeometry(g);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *m = new QSGFlatColorMaterial;
        m->setColor(lineColor_);
        node->setMaterial(m);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else {
        node->geometry()->setLineWidth(realLineWidth());
        QSGFlatColorMaterial *m = static_cast <QSGFlatColorMaterial *> (node->material());
        m->setColor(lineColor_);
    }

    const int count = 6;
    node->geometry()->allocate(count);
    QSGGeometry::Point2D *vertexes = node->geometry()->vertexDataAsPoint2D();

    switch (indicatorForm_) {
        case IndicatorForm::Arrow: {
            // 0
            vertexes[0].set(0, rect.height() * 0.5);
            vertexes[1].set(rect.width(), rect.height() * 0.5);

            double angle = 30;
            double arrowLineSize = rect.height() * 0.5 / std::sin(qDegreesToRadians(angle)); // * 0.5 * 0.5;//* 1.4142;    // sqrt(2)

            double arrowX = std::sqrt(arrowLineSize * arrowLineSize - rect.height() * rect.height() * 0.5 * 0.5);
            double arrowY = 0;
            // 2
            vertexes[2].set(rect.width(), rect.height() * 0.5);
            vertexes[3].set(rect.width() - arrowX, std::max(0., arrowY));

            // 3
            vertexes[4].set(rect.width(), rect.height() * 0.5);
            vertexes[5].set(rect.width() - arrowX, std::min(rect.height(), rect.height() - arrowY));
            break;
        }
        case IndicatorForm::Cross: {
            // 0
            vertexes[0].set(0, rect.height() * 0.5 - realLineWidth() * 0.5);
            vertexes[1].set(rect.width() - rect.height() * 0.5, rect.height() * 0.5 - realLineWidth() * 0.5);

            // 2
            vertexes[2].set(rect.width() - rect.height(), 0);
            vertexes[3].set(rect.width(), rect.height());

            // 3
            vertexes[4].set(rect.width() - rect.height(), rect.height());
            vertexes[5].set(rect.width(), 0);

            break;
        }
        default:
            break;
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

ArrowItem::IndicatorForm ArrowItem::indicatorForm() const
{
    return indicatorForm_;
}

void ArrowItem::setIndicatorForm(const IndicatorForm &indicatorForm)
{
    if(indicatorForm_ != indicatorForm) {
        indicatorForm_ = indicatorForm;
        emit indicatorFormChanged();
        update();
    }
}

QColor ArrowItem::arrowColor() const
{
    return arrowColor_;
}

void ArrowItem::setArrowColor(const QColor &arrowColor)
{
    if(arrowColor_ != arrowColor) {
        arrowColor_ = arrowColor;
        emit colorChanged();
        update();
    }
}

QColor ArrowItem::lineColor() const
{
    return lineColor_;
}

void ArrowItem::setLineColor(const QColor &lineColor)
{
    if(lineColor_ != lineColor) {
        lineColor_ = lineColor;
        emit colorChanged();
        update();
    }
}

qreal ArrowItem::realLineWidth() const
{
    return std::min(lineWidth_, height());
}
