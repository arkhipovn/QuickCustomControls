#ifndef GRADIENTIMAGE_H
#define GRADIENTIMAGE_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QTimerEvent>
#include <QEasingCurve>
#include <QPropertyAnimation>

class GradientImage : public QQuickPaintedItem
{
    Q_OBJECT

public:
    GradientImage(QQuickItem *parent = nullptr);
    ~GradientImage();

public slots:
    QColor getColor(int x, int y);

protected:
    void paint(QPainter *painter);
    void timerEvent(QTimerEvent*);
    void geometryChanged(const QRectF &newGeometry,
                         const QRectF &oldGeometry);

private:
    QImage image;  // пока не знаю как сделать..
    int timerId;

    int prX;
    int progressbarWidth;
    int duration = 1000;
    int step;
    int timerInterval = 25;

    QEasingCurve curve;
    qreal interval;
    qreal progress;

    QPropertyAnimation *animator;
};

#endif // GRADIENTIMAGE_H
