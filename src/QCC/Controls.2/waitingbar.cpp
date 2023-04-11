#include "waitingbar.h"
#include <QLinearGradient>
#include <QDebug>

WaitingBar::WaitingBar(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true);

    setHeight(5);
    setClip(true);
    progressColor = Qt::blue;
    color = Qt::white;


    timer = new QTimer(this);
    timer->setInterval(25); // 25?
    QObject::connect(timer, &QTimer::timeout, this, &WaitingBar::timeout);

    //visibl
    easingCurve.setType(QEasingCurve::OutInSine);

    duration = 1000;
    progress = 0;
    interval = timer->interval() / (double) duration;
    progressBarWidth = width() * 0.2;
}

WaitingBar::~WaitingBar()
{
    if(timer->isActive()) timer->stop();
}

//void WaitingBar::componentComplete()
//{
//    //    progressBarWidth = width() * 0.2;
//}

void WaitingBar::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(oldGeometry)
    setProgressBarWidth(newGeometry.width() * 0.2); //@N..
}

void WaitingBar::timeout()
{
    progress += interval;
    if(progress > 1) progress = 0;
    update();
}

QColor WaitingBar::getProgressColor() const
{
    return progressColor;
}

void WaitingBar::setProgressColor(const QColor &value)
{
    if(progressColor != value) {
        progressColor = value;
        emit progressColorChanged();
    }
}

void WaitingBar::setVisible(bool value)
{
    if(!value) setRunning(false);
    QQuickPaintedItem::setVisible(value);
}

void WaitingBar::start()
{
    setRunning(true);
}

void WaitingBar::stop()
{
    setRunning(false);
}

QColor WaitingBar::getColor() const
{
    return color;
}

void WaitingBar::setColor(const QColor &value)
{
    if(color != value) {
        color = value;
        emit colorChanged();
    }
}

qreal WaitingBar::getProgressBarWidth() const
{
    return progressBarWidth;
}

void WaitingBar::setProgressBarWidth(qreal value)
{
    if(progressBarWidth != value) {
        progressBarWidth = value;
        emit progressBarWidthChanged();
    }
}

int WaitingBar::getDuration() const
{
    return duration;
}

void WaitingBar::setDuration(int value)
{
    if(duration != value) {
        duration = value;
        interval = timer->interval() / (double) duration;
        emit durationChanged();
    }
}

bool WaitingBar::getRunning() const
{
    return timer->isActive();
}

void WaitingBar::setRunning(bool value)
{
    if(timer->isActive() == value) return;
    progress = 0;
    if(value) timer->start();
    else {
        timer->stop();
        update();
    }
    emit runningChanged();
}


void WaitingBar::paint(QPainter *painter)
{
    QRectF boundingrect = boundingRect();
    painter->fillRect(boundingrect, color);

    // max: width() + progressbarWidth
    // min: - progressbarWidth
    // (max - min) + min
    qreal progressX = easingCurve.valueForProgress(progress) * (width() + progressBarWidth) - progressBarWidth;

    QRectF progressBarRect(progressX, 0, progressBarWidth, boundingrect.height());
    QLinearGradient gradient(progressX, 0, progressX + progressBarWidth, 0);
    QGradientStops stops;
    stops.append(QGradientStop(0.0, color));
    stops.append(QGradientStop(0.5, progressColor));
    stops.append(QGradientStop(1.0, color));
    gradient.setStops(stops);
    painter->fillRect(progressBarRect, gradient);
}



