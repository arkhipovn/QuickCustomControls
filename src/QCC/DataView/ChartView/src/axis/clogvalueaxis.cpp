#include "clogvalueaxis.h"

CLogValueAxis::CLogValueAxis(QQuickItem *parent) :
    CAxis(LogValueAxisType, parent)
{
    tickCountY = 0;
    tickCountX = 0;

    base = 10;
    domain = new CXYDomain(this);
    QObject::connect(domain, SIGNAL(updated()), this, SLOT(update()));
    QObject::connect(domain, &CDomain::updated, this, &CAxis::axisRangeChanged);
    QObject::connect(domain, &CDomain::rangeVerticalChanged, this, &CAxis::rangeVerticalChanged);
    QObject::connect(domain, &CDomain::rangeHorizontalChanged, this, &CAxis::rangeHorizontalChanged);
}

int CLogValueAxis::getBase() const
{
    return base;
}

QPointF CLogValueAxis::mapToPosition(const QPointF &value)
{
    if(value.x() > 0 && value.y() > 0) {
        qreal valuex = value.x() < 0 ? 1 : qLn(value.x()) / qLn(base);
        qreal valuey = value.y() < 0 ? 1 : qLn(value.y()) / qLn(base);
        qreal maxx = domain->getMaxX() < 0 ? 1 : (qLn(domain->getMaxX()) / qLn(base));
        qreal minx = domain->getMinX() < 0 ? 1 : (qLn(domain->getMinX()) / qLn(base));
        qreal maxy = domain->getMaxY() < 0 ? 1 : (qLn(domain->getMaxY()) / qLn(base));
        qreal miny = domain->getMinY() < 0 ? 1 : (qLn(domain->getMinY()) / qLn(base));

        qreal nx = (valuex - minx) / (maxx - minx);
        qreal ny = 1 - (valuey - miny) / (maxy - miny);

        qreal x = plotArea.x() + nx * plotArea.width();
        qreal y = plotArea.y() + ny * plotArea.height();

        return QPointF(x, y);
    }
    return QPointF(1, 1);
}

QPointF CLogValueAxis::mapToValue(const QPointF &point)
{
    qreal nx = (point.x() - plotArea.x()) / plotArea.width();
    qreal ny = 1 - (point.y() - plotArea.y()) / plotArea.height();

    qreal maxx = domain->getMaxX() < 0 ? 1 : (qLn(domain->getMaxX()) / qLn(base));
    qreal minx = domain->getMinX() < 0 ? 1 : (qLn(domain->getMinX()) / qLn(base));
    qreal maxy = domain->getMaxY() < 0 ? 1 : (qLn(domain->getMaxY()) / qLn(base));
    qreal miny = domain->getMinY() < 0 ? 1 : (qLn(domain->getMinY()) / qLn(base));
    return QPointF(qPow(10, nx * (maxx - minx) + minx), qPow(10, ny * (maxy - miny) + miny));
}

void CLogValueAxis::setMinX(const qreal &min)
{
    if(min < 1e-20) domain->setMinX(1e-20);
    else domain->setMinX(min);
    //    if(qFuzzyCompare(min + 1, 1.)) domain->setMinX(1e-20);
    //    else domain->setMinX(min);
}

void CLogValueAxis::setMaxX(const qreal &max)
{
    if(max < 1e-20) domain->setMaxX(1e-20);
    else domain->setMaxX(max);
    //    if(qFuzzyCompare(min + 1, 1.)) domain->setMaxX(1e-20);
    //    else domain->setMaxX(min);
}

void CLogValueAxis::setMinY(const qreal &min)
{
    if(min < 1e-20) domain->setMinY(1e-20);
    else domain->setMinY(min);
    //    if(qFuzzyCompare(min + 1, 1.)) domain->setMinY(1e-20);
    //    else domain->setMinY(min);
}

void CLogValueAxis::setMaxY(const qreal &max)
{
    if(max < 1e-20) domain->setMaxY(1e-20);
    else domain->setMaxY(max);
    //    if(qFuzzyCompare(min + 1, 1.)) domain->setMaxY(1e-20);
    //    else domain->setMaxY(min);
}

QString CLogValueAxis::toStrValue(qreal value, QRect &bound) {
    QString strvalue = QString::number(value);
    bound = getFontMetrix(labelsFont, strvalue);
    return strvalue;
}

void CLogValueAxis::paint(QPainter *painter)
{
    //@N проверить с большими числами...

    painter->setFont(labelsFont);
    calcAxisSettings(); // @N перенести на изменение мин, макс

    QRectF boundingrect = boundingRect();

    // расчитаем высоту и ширину текста по оси Y
    int maxheight = 0;
    int maxwidth = 0;
    int temp = 0;

    if(tickCountY > 0) {
        // Текст
        QStringList list;
        QList <qreal> valuesY;
        temp = qCeil(lnAxisY.x());
        qreal boundvalue;
        qreal prevvalue = pow(base, temp - 1);

        int minorTikStart = 2;
        int minorTikInterval = 2;

        switch (tickCountY) {
            case 0:
            case 1:
                minorTikInterval = 1;
                break;
            case 2:
                minorTikInterval = 3;
                break;
            case 3: break;
            case 4:
                minorTikInterval = 3;
                break;
            case 5:
            case 6:
            case 7:
                minorTikStart = 5;
                minorTikInterval = 5;
                break;
            default:
                minorTikStart = 10;
                break;
        }

        for(int i = 0; i <= tickCountY; ++i) {
            qreal value = pow(base, temp + i);

            if(i == 0) boundvalue = pow(base, lnAxisY.x());
            else if(i == tickCountY) boundvalue = pow(base, lnAxisY.y());
            else boundvalue = value;

            for (int minorTik = minorTikStart; minorTik < 10; minorTik += minorTikInterval) {
                qreal v = prevvalue * minorTik;

                if((i == 0 && v > boundvalue) || (i != 0 && v < boundvalue)) {
                    if(v > 0) {
                        valuesY.append(qLn(v) / qLn(base));

                        // нужна или нет?
                        QRect fontRect;
                        list.append(toStrValue(v, fontRect));
                        if(maxwidth < fontRect.width()) maxwidth = fontRect.width();
                        if(maxheight < fontRect.height()) maxheight = fontRect.height();
                    }
                }
            }

            if(i < tickCountY) {
                QRect fontRect;
                list.append(toStrValue(value, fontRect));
                valuesY.append(temp + i);
                if(maxwidth < fontRect.width()) maxwidth = fontRect.width();
                if(maxheight < fontRect.height()) maxheight = fontRect.height();
            }
            prevvalue = value;
        }

        for(int i = 0; i < valuesY.size(); ++i) {
            qreal dh = plotArea.height() * (1 - (valuesY.at(i) - lnAxisY.x()) / (lnAxisY.y() - lnAxisY.x()));

            // сетка
            QPointF p2(plotArea.x() + DASH_SIZE, plotArea.y() + dh);
            QPointF p3(plotArea.x() + plotArea.width() - DASH_SIZE, plotArea.y() + dh);
            painter->setPen(gridPen);
            painter->drawLine(p2, p3);

            if(!list.at(i).isEmpty()) {
                // текст
                QPointF p1(plotArea.x() - maxwidth - DASH_SIZE, plotArea.y() + dh);
                painter->setPen(labelsPen);
                painter->drawText(p1, list.at(i));

                // маленькие черточки
                QPointF p4(plotArea.x(), plotArea.y() + dh);
                QPointF p5(plotArea.x() + plotArea.width(), plotArea.y() + dh);
                painter->setPen(gridSolidPen);
                painter->drawLine(p4, QPointF(p4.x() + DASH_SIZE, p4.y()));
                painter->drawLine(QPointF(p5.x() - DASH_SIZE, p5.y()), p5);
            }
        }
    }
    else {
        maxheight = 10;
        maxwidth = 10;
    }

    QMarginsF margins(maxwidth + 2 * DASH_SIZE, maxheight * 1.5, maxwidth * 0.5, maxheight * 2);
    setPlotArea(boundingrect.marginsRemoved(margins));

    if(tickCountX > 0) {
        QStringList list;
        QList <qreal> valuesX;
        temp = qCeil(lnAxisX.x());
        qreal boundvalue;
        qreal prevvalue = pow(base, temp - 1);

        for(int i = 0; i <= tickCountX; ++i) {
            qreal value = pow(base, temp + i);

            if(i == 0) boundvalue = pow(base, lnAxisX.x());
            else if(i == tickCountX) boundvalue = pow(base, lnAxisX.y());
            else boundvalue = value;

            for(int coef = 2; coef < 10; coef += 2) {
                qreal v = prevvalue * coef;

                if((i == 0 && v > boundvalue) || (i != 0 && v < boundvalue)) {
                    if(v > 0) {
                        valuesX.append(qLn(v) / qLn(base));
                        list.append(""); // подпись не нужна..
                    }
                    //                    list.append(QString::number(v)); // подпись не нужна..
                }
            }

            if(i < tickCountX) {
                list.append(QString::number(value));
                valuesX.append(temp + i);
            }

            prevvalue = value;
        }

        for(int i = 0; i < valuesX.size(); ++i) {
            qreal dw = plotArea.width() * (valuesX.at(i) - lnAxisX.x()) / (lnAxisX.y() - lnAxisX.x());

            // сетка
            QPointF p2(plotArea.x() + dw, plotArea.y() + DASH_SIZE);
            QPointF p3(plotArea.x() + dw, plotArea.y() + plotArea.height() - DASH_SIZE);
            painter->setPen(gridPen);
            painter->drawLine(p2, p3);

            if(!list.at(i).isEmpty()) {
                QRect fontRect = getFontMetrix(labelsFont, list.at(i));
                qreal px = plotArea.x() + dw - fontRect.width() * 0.5;
                if(px < boundingrect.x()) px = boundingrect.x() + DASH_SIZE;
                else if(px > boundingrect.x() + boundingrect.width() - fontRect.width()) px = boundingrect.x() + boundingrect.width() - fontRect.width() - DASH_SIZE;

                QPointF p1(px, plotArea.y() + plotArea.height() + fontRect.height() + DASH_SIZE);
                painter->setPen(labelsPen);
                painter->drawText(p1, list.at(i));

                // маленькие черточки
                QPointF p4(plotArea.x() + dw, plotArea.y());
                QPointF p5(plotArea.x() + dw, plotArea.y() + plotArea.height());
                painter->setPen(gridSolidPen);
                painter->drawLine(p4, QPointF(p4.x(), p4.y() + DASH_SIZE));
                painter->drawLine(QPointF(p5.x(), p5.y() - DASH_SIZE), p5);
            }
        }
    }

    // Рамка вокруг
    painter->setPen(axisPen);
    painter->drawRect(plotArea);
}

void CLogValueAxis::calcAxisSettings()
{
    const qreal logMaxX = qLn(domain->getMaxX()) / qLn(base);
    const qreal logMinX = qLn(domain->getMinX()) / qLn(base);
    lnAxisX = QPointF(logMinX, logMaxX);
    int tickCount = qAbs(qCeil(logMaxX) - qCeil(logMinX));

    qreal highValue = logMinX < logMaxX ? logMaxX : logMinX;
    if(qFuzzyCompare(highValue, qreal(qCeil(highValue))))
        ++tickCount;
    //    qDebug() << tickCount << logMinX << logMaxX << domain->getMaxX() << domain->getMinX() << base;
    setTickCountX(tickCount);

    const qreal logMaxY = qLn(domain->getMaxY()) / qLn(base);
    const qreal logMinY = qLn(domain->getMinY()) / qLn(base);
    lnAxisY = QPointF(logMinY, logMaxY);

    tickCount = qAbs(qCeil(logMaxY) - qCeil(logMinY));

    highValue = logMinY < logMaxY ? logMaxY : logMinY;
    if(qFuzzyCompare(highValue, qreal(qCeil(highValue))))
        ++tickCount;
    //    qDebug() << tickCount << logMinY << logMaxY << domain->getMaxY() << domain->getMinY();
    setTickCountY(tickCount);
}


//void CLogValueAxis::calcAxisSettings()
//{
//    //    const qreal logMaxX = domain->getMaxX() < 1e-20 ? 1 : (qLn(domain->getMaxX()) / qLn(base));
//    //    const qreal logMinX = domain->getMinX() < 1e-20 ? 1 : (qLn(domain->getMinX()) / qLn(base));
//    const qreal logMaxX = qFuzzyCompare(domain->getMaxX() + 1, 1.) ? 1 : (qLn(domain->getMaxX()) / qLn(base));
//    const qreal logMinX = qFuzzyCompare(domain->getMinX() + 1, 1.) ? 1 : (qLn(domain->getMinX()) / qLn(base));
//    lnAxisX = QPointF(logMinX, logMaxX);
//    int tickCount = qAbs(qCeil(logMaxX) - qCeil(logMinX));

//    qreal highValue = logMinX < logMaxX ? logMaxX : logMinX;
//    if(qFuzzyCompare(highValue, qreal(qCeil(highValue))))
//        ++tickCount;
//    //    qDebug() << tickCount << logMinX << logMaxX << domain->getMaxX() << domain->getMinX() << base;
//    setTickCountX(tickCount);

//    const qreal logMaxY = qFuzzyCompare(domain->getMaxY() + 1, 1.) ? 1 : (qLn(domain->getMaxY()) / qLn(base));
//    const qreal logMinY = qFuzzyCompare(domain->getMinY() + 1, 1.) ? 1 : (qLn(domain->getMinY()) / qLn(base));
//    lnAxisY = QPointF(logMinY, logMaxY);

//    tickCount = qAbs(qCeil(logMaxY) - qCeil(logMinY));

//    highValue = logMinY < logMaxY ? logMaxY : logMinY;
//    if(qFuzzyCompare(highValue, qreal(qCeil(highValue))))
//        ++tickCount;
//    //    qDebug() << tickCount << logMinY << logMaxY << domain->getMaxY() << domain->getMinY();
//    setTickCountY(tickCount);
//}

