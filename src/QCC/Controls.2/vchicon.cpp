#include "vchicon.h"
#include <QPainter>
#include <QImage>
//#include <QPainterPath>

VchIcon::VchIcon(QQuickItem *parent) : QQuickPaintedItem(parent), timeSpec(Qt::LocalTime), datetime(QDateTime::currentDateTime().toTimeSpec(timeSpec))
{
    timer.setInterval(100);
    QObject::connect(&timer, &QTimer::timeout, this, &VchIcon::timeout);
}

void VchIcon::timeout()
{
    setDateTime(QDateTime::currentDateTime());
}

void VchIcon::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QImage img(":/images/vchicon.png");
    img = img.scaled(size().width(), size().height(),
                     Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QRectF source(0.0, 0.0, img.width(), img.height());
    int deltaX = source.width() < boundingRect().width() ? boundingRect().width() - source.width()
                                                         : source.width() - boundingRect().width();
    int deltaY = source.height() < boundingRect().height() ? boundingRect().height() - source.height()
                                                           : source.height() - boundingRect().height();

    painter->translate(deltaX / 2, deltaY / 2);

    //deltaX
    // background
    painter->drawImage(source, img);


    constexpr qreal horCenter = 78 / 182.;
    painter->translate(source.width() / 2., horCenter * source.height());    // центр часов
    painter->rotate(180);

    // Shorthand
    QPen shorthandPen("#997a7a");
    painter->setBrush(QBrush("#b38080"));
    painter->setPen(shorthandPen);
    qreal shorthandWidth = source.width() * 0.035;
    qreal shorthandHeight = source.height() * 0.27;

    painter->save();
    painter->rotate(datetime.time().hour() * 30);
    painter->drawRect(-shorthandWidth * 0.5, -shorthandHeight * 0.2,
                      shorthandWidth, shorthandHeight);
    painter->restore();


    // longhand
    qreal longhandWidth = source.width() * 0.02;
    qreal longhandHeight = source.height() * 0.35;
    painter->save();
    painter->rotate(datetime.time().minute() * 6 + 3 * (datetime.time().second() < 30 ? 0 : 1));
    painter->drawRect(-longhandWidth * 0.5, -longhandHeight * 0.2,
                      longhandWidth, longhandHeight);
    painter->restore();


    // secondhand
    QPen secondhandPen("#8A2900");
    painter->setBrush(QBrush("#BD3A00"));
    painter->setPen(secondhandPen);

    painter->save();
    qreal secondhandWidth = source.width() * 0.015;
    qreal secondhandHeight = source.height() * 0.42;
    painter->rotate(datetime.time().second() * 6);

    QPolygonF p;
    p << QPointF(-secondhandWidth * 0.5, - secondhandHeight * 0.2)
      << QPointF(secondhandWidth * 0.5, - secondhandHeight * 0.2)
      << QPointF(0, secondhandHeight - secondhandHeight * 0.2);
    painter->drawPolygon(p);


    painter->setBrush(Qt::yellow);
    painter->drawEllipse(-secondhandWidth, -secondhandWidth + secondhandHeight * 0.5,
                         secondhandWidth * 2, secondhandWidth * 2);

    painter->restore();


    painter->setBrush(QBrush("#BD3A00"));
    painter->drawEllipse(-secondhandWidth, -secondhandWidth, secondhandWidth * 2, secondhandWidth * 2);





    //    constexpr qreal temp1 = 4 / 170.;


    //    qreal temp = qMin(boundingRect().size().width(), boundingRect().size().height());
    //    QPen pen("blue");
    //    pen.setWidth(temp * temp1);
    //    painter->setPen(pen);

    //    constexpr qreal k1 = 21 / 170.;
    //    qreal p1w = temp - pen.width();
    //    auto p1 = getPoligon(QRectF(QPointF(pen.width() / 2., pen.width() / 2.), QSizeF(p1w, p1w)), k1);
    //    painter->drawConvexPolygon(p1);


    //    constexpr qreal k2 = 15 / 153.;
    //    qreal p2w = temp - pen.width() - /* вычесть расстояние между полигонами*/ 4 * pen.width();
    //    auto p2 = getPoligon(QRectF(QPointF(2 * pen.width() + pen.width() / 2., 2 * pen.width() + pen.width() / 2.), QSizeF(p2w, p2w)), k2);
    //    painter->drawConvexPolygon(p2);

    //    constexpr qreal const1 = 76 / 170.;
    //    qreal chw = temp * const1 - pen.width();
    //    QRectF f = QRectF(QPointF(pen.width() / 2, pen.width() / 2), QSizeF(chw, chw));
    //    constexpr qreal constCh = 76;


    //    // Vch
    //    QPainterPath ch;
    //    ch.moveTo(f.x() + 24. / constCh * f.width(), f.y() + 56 / constCh * f.height());
    //    ch.lineTo(f.x() + 44. / constCh * f.width(), f.y() + 56 / constCh * f.height());
    //    ch.lineTo(f.x() + 44. / constCh * f.width(), f.y() + f.height());
    //    ch.lineTo(f.x() + 64. / constCh * f.width(), f.y() + f.height());
    //    ch.lineTo(f.x() + f.width(), f.y() + 52 / constCh * f.height());
    //    ch.lineTo(f.x() + 64. / constCh * f.width(), f.y() + 52 / constCh * f.height());
    //    ch.lineTo(f.x() + 64. / constCh * f.width(), f.y());
    //    ch.lineTo(f.x() + 46 / constCh * f.width(), f.y());
    //    ch.lineTo(f.x() + 46 / constCh * f.width(), f.y() + 34 / constCh * f.height());
    //    ch.lineTo(f.x() + 34 / constCh * f.width(), f.y() + 34 / constCh * f.height());
    //    ch.lineTo(f.x() + 34 / constCh * f.width(), f.y());
    //    ch.lineTo(f.x() + 12 / constCh * f.width(), f.y());
    //    ch.lineTo(f.x(), f.y() + 24 / constCh * f.height());
    //    ch.lineTo(f.x() + 12 / constCh * f.width(), f.y() + 24 / constCh * f.height());
    //    ch.lineTo(f.x() + 12 / constCh * f.width(), f.y() + 44 / constCh * f.height());


    //    //    ch.arcTo(f.x() + 13. / constCh * f.width(), f.y() + 48. / constCh * f.height(), 13. / constCh * f.width(), 13. / constCh * f.height(), 180, 90);

    //    painter->strokePath(ch, pen);



    //    painter->drawImage(boundingRect(), img);

    /*IgnoreAspectRatio,
                            KeepAspectRatio,
                            KeepAspectRatioByExpanding
                            */

    //    painter->drawPixmap(QPoint(0,0), QPixmap(":/images/vchicon.png"));
    //    KeepAspectRatio
}

QDateTime VchIcon::getDateTime() const
{
    return datetime;
}

void VchIcon::setDateTime(const QDateTime &value)
{
    if(datetime != value) {
        datetime = datetime.timeSpec() == value.timeSpec() ? value
                                                           : value.toTimeSpec(timeSpec);
        update();
    }
}

Qt::TimeSpec VchIcon::getTimeSpec() const
{
    return timeSpec;
}

void VchIcon::setTimeSpec(const Qt::TimeSpec &value)
{
    if(timeSpec != value) {
        timeSpec = value;
        datetime = datetime.toTimeSpec(timeSpec);
        update();
        emit timeSpecChanged();
    }
}

int VchIcon::interval() const
{
    return timer.interval();
}

void VchIcon::setInterval(int msec)
{
    if(timer.interval() != msec) {
        timer.setInterval(msec);
        emit intervalChanged();
    }
}

bool VchIcon::isActive() const
{
    return timer.isActive();
}

void VchIcon::setActive(bool active)
{
    if(timer.isActive() == active) return;
    if(timer.isActive()) timer.stop();
    else timer.start();
    emit activeChanged();
}

void VchIcon::start()
{
    if(timer.isActive()) return;
    timer.start();
    emit activeChanged();
}

void VchIcon::stop()
{
    if(!timer.isActive()) return;
    timer.stop();
    emit activeChanged();
}

QPolygonF VchIcon::getPoligon(const QRectF &rec, qreal temp1) const
{
    QPolygonF p;

    // правый угол
    p.append(QPointF(rec.x() + rec.width() * (1 - temp1), rec.y()));
    p.append(QPointF(rec.x() + rec.width(), rec.y() + rec.height() * temp1));
    p.append(QPointF(rec.x() + rec.width(), rec.y() + rec.height() * (1 - temp1)));
    p.append(QPointF(rec.x() + rec.width() * (1 - temp1), rec.y() + rec.height()));

    // левый угол
    p.append(QPointF(rec.x() + rec.width() * temp1, rec.y() + rec.height()));
    p.append(QPointF(rec.x(), rec.y() + rec.height() * (1 - temp1)));
    p.append(QPointF(rec.x(), rec.y() + rec.height() * temp1));
    p.append(QPointF(rec.x() + rec.width() * temp1, rec.y()));

    return p;
}


