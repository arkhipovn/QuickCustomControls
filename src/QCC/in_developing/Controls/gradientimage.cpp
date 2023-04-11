#include "gradientimage.h"
#include <QGradient>
#include <QLinearGradient>
#include <QImage>

GradientImage::GradientImage(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true); //@N

    //QQuickPaintedItem::Image
    //RenderTarget
}
#include <QSharedPointer>
#include <QDebug>
QColor GradientImage::getColor(int x, int y)
{
    //    qDebug() << "getCOlor" << image.pixel(x, y);
    if(image.isNull()) return Qt::white;
    return QColor(image.pixel(x, y));
}
#include <QQuickItemGrabResult>
void GradientImage::paint(QPainter *painter)
{
    QRectF boundingrect = boundingRect();

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


    qDebug() << grabToImage().data()->image() << grabToImage().data()->url();
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


/*
0,0: (  0, 79,150,  0) #004F9600
1,0: (  0, 81,150,  0) #00519600
2,0: (  1, 83,150,  0) #01539600
3,0: (  1, 85,150,  0) #01559600
4,0: (  3, 87,152,  0) #03579800
5,0: (  2, 89,151,  0) #02599700
6,0: (  2, 92,152,  0) #025C9800
7,0: (  2, 93,151,  0) #025D9700
8,0: (  3, 96,151,  0) #03609700
9,0: (  4, 99,151,  0) #04639700
0,1: (  0, 79,152,  0) #004F9800
1,1: (  0, 81,151,  0) #00519700
2,1: (  1, 83,151,  0) #01539700
3,1: (  2, 85,152,  0) #02559800
4,1: (  3, 88,155,  0) #03589B00
5,1: (  2, 89,154,  0) #02599A00
*/
