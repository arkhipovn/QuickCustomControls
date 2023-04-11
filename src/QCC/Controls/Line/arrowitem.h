#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QQuickItem>

//@N добавить зависимость от ширины линии. Чтобы было по центру и тд

class ArrowItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY colorChanged)
//    Q_PROPERTY(QColor arrowColor READ arrowColor WRITE setArrowColor NOTIFY colorChanged) // indicatorColor
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

    Q_PROPERTY(IndicatorForm indicatorForm READ indicatorForm WRITE setIndicatorForm NOTIFY indicatorFormChanged)

public:
    explicit ArrowItem(QQuickItem *parent = nullptr);

    enum class IndicatorForm {
        Arrow,
        Cross
    };
    Q_ENUMS(IndicatorForm)

    qreal lineWidth() const;
    void setLineWidth(const qreal &lineWidth);

    QColor lineColor() const;
    void setLineColor(const QColor &lineColor);

    QColor arrowColor() const;
    void setArrowColor(const QColor &arrowColor);

    IndicatorForm indicatorForm() const;
    void setIndicatorForm(const IndicatorForm &indicatorForm);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

signals:
    void colorChanged();
    void lineWidthChanged();
    void indicatorFormChanged();

private:
    QColor lineColor_;
    QColor arrowColor_;
    qreal lineWidth_;
    IndicatorForm indicatorForm_;

    qreal realLineWidth() const;
};

#endif // ARROWITEM_H
