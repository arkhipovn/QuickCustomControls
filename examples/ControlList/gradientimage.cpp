#include "gradientimage.h"
#include <QGradient>
#include <QLinearGradient>
#include <QImage>
#include <QSharedPointer>
#include <QDebug>

GradientImage::GradientImage(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    progress = 0;

    //    animator = new QPropertyAnimation(this);
    //    animator->setDuration(1000);
    //    animator->setEasingCurve(QEasingCurve::InOutQuad);

    //    progressbarWidth = width() * 0.2;
    //    animator->setStartValue(0);
    //    animator->setEndValue(1);
    //    animator->start();

    curve.setType(QEasingCurve::OutInSine);


    progressbarWidth = width() * 0.2;
    step = width() / (duration / timerInterval);
    interval = timerInterval / (double)duration;
    qDebug() << timerInterval << duration << interval;

    prX = -progressbarWidth;

    setFlag(ItemHasContents, true); //@N

    //QQuickPaintedItem::Image
    //RenderTarget
    timerId = QObject::startTimer(timerInterval);
}

GradientImage::~GradientImage()
{
    //    QObject::killTimer(timerId);
}


QColor GradientImage::getColor(int x, int y)
{
    //    qDebug() << "getCOlor" << image.pixel(x, y);
    if(image.isNull()) return Qt::white;
    return QColor(image.pixel(x, y));
}


void GradientImage::paint(QPainter *painter)
{
    QColor backgr = Qt::white;

    QRectF boundingrect = boundingRect();
    painter->fillRect(boundingrect, backgr);


    QRectF rr(prX, 0, progressbarWidth, boundingrect.height());
    QLinearGradient gradient(prX, 0, prX + progressbarWidth, 0);
    QGradientStops stops;
    stops.append(QGradientStop(0.0, backgr));
    stops.append(QGradientStop(0.5, "blue"));
    stops.append(QGradientStop(1.0, backgr));
    gradient.setStops(stops);

    //    QRectF rr(prX, 0, progressbarWidth, boundingrect.height());
    painter->fillRect(rr, gradient);


    /*
    int offset = 1;
    QLinearGradient horGradient(offset, 0, boundingrect.width() - offset, 0);
    QGradientStops stops;
    stops.append(QGradientStop(0.0, "#ff0000"));
    stops.append(QGradientStop(0.2, "#ffff00"));
    stops.append(QGradientStop(0.4, "#00ff00"));
    stops.append(QGradientStop(0.6, "#00ffff"));
    stops.append(QGradientStop(0.7, "#0000ff"));
    stops.append(QGradientStop(1.0, "#ff00ff"));
    horGradient.setStops(stops);

    QLinearGradient verGradient(0, offset, 0, boundingrect.height() - offset);
    QGradientStops stops2;
    stops2.append(QGradientStop(0.0, Qt::black));
    stops2.append(QGradientStop(0.4, Qt::transparent));
    stops2.append(QGradientStop(1.0, Qt::white));
    verGradient.setStops(stops2);

    painter->fillRect(boundingrect, horGradient);
    painter->fillRect(boundingrect, verGradient);


    qDebug() << grabToImage().data()->image() << grabToImage().data()->url();*/
    //    qDebug() << grabToImage().data()->;
    //    image = QImage((int)boundingrect.width(), (int)boundingrect.height(), QImage::Format_ARGB32);
    //    painter->drawImage(boundingrect, image);
    //    //    this->painter = painter;

    //    qDebug() << image;
    //gra
    //QImage.pix
    //    painter->drawImage();
    //        const pixel = ctx.getImageData(x, y, 1, 1)
    //        const data = pixel.data
}

void GradientImage::timerEvent(QTimerEvent *)
{
    // max: width() + progressbarWidth
    // min: - progressbarWidth
    // (max - min) + min
    progress += interval;
    if(progress > 1) progress = 0;

    prX = curve.valueForProgress(progress) * (width() + progressbarWidth) - progressbarWidth;
    update();
}

void GradientImage::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    progressbarWidth = newGeometry.width() * 0.2;
    step = newGeometry.width() / (duration / timerInterval);
}

