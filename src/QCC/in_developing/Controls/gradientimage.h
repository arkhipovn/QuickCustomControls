#ifndef GRADIENTIMAGE_H
#define GRADIENTIMAGE_H

#include <QQuickPaintedItem>
#include <QPainter>

class GradientImage : public QQuickPaintedItem
{
    Q_OBJECT

public:
    GradientImage(QQuickItem *parent = nullptr);

public slots:
    QColor getColor(int x, int y);

protected:
    void paint(QPainter *painter);

private:
    QImage image;  // пока не знаю как сделать..
};

#endif // GRADIENTIMAGE_H
