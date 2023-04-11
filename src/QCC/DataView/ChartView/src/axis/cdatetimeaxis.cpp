#include "cdatetimeaxis.h"

CDateTimeAxis::CDateTimeAxis(QQuickItem *parent) :
    CValueAxis(DateTimeAxisType, parent), timeSpec(Qt::UTC), dateFormat("dd:MM:yyyy")
{

}

QString CDateTimeAxis::getDateFormat() const
{
    return dateFormat;
}

void CDateTimeAxis::setDateFormat(const QString &value)
{
    if(dateFormat != value) {
        dateFormat = value;
        emit dateFormatChanged();
        update();
    }
}

Qt::TimeSpec CDateTimeAxis::getTimeSpec() const
{
    return timeSpec;
}

void CDateTimeAxis::setTimeSpec(Qt::TimeSpec timespec)
{
    if(timespec != timeSpec) {
        timeSpec = timespec;
        emit timeSpecChanged();
        update();
    }
}

QString CDateTimeAxis::secsToString(qint64 secs, const QString &format) const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 8, 0)
    return QDateTime::fromMSecsSinceEpoch(secs * 1000, timeSpec).toString(format);
#else
    return QDateTime::fromSecsSinceEpoch(secs, timeSpec).toString(format);
#endif
}

void CDateTimeAxis::paint(QPainter *painter)
{
    painter->setFont(labelsFont);

    QRectF boundingrect = boundingRect();

    // Текст
    painter->setPen(labelsPen);

    bool dyIsNull = qFuzzyCompare(1 + interval.y(), 1);
    bool dxIsNull = qFuzzyCompare(1 + interval.x(), 1);

    // расчитаем высоту и ширину текста по оси Y
    int maxLabelHeight = 0;
    int maxLabelWidth = 0;
    QStringList axisYvalues;
    qreal startValue = 0;

    if(!dyIsNull) { // не равно 0
        startValue = (domain->getMinY() - offset) / factor;
        for(int i = 0; i <= tickCountY + 1; ++i){
            float value = startValue + (tickCountY + 1 - i) * interval.y();
            QString strvalue = QString::number(value, 'f', precision);
            axisYvalues.append(strvalue);
            QRect fontRect = getFontMetrix(labelsFont, strvalue);
            if(maxLabelWidth < fontRect.width()) maxLabelWidth = fontRect.width();
            if(maxLabelHeight < fontRect.height()) maxLabelHeight = fontRect.height();
        }
    }

    QMarginsF margins(maxLabelWidth + 2 * DASH_SIZE, maxLabelHeight * 2, maxLabelWidth * 0.5, maxLabelHeight * 2);
    setPlotArea(boundingrect.marginsRemoved(margins));

    float dw = plotArea.width() / (double)(tickCountX + 1);
    float dh = plotArea.height() / (double)(tickCountY + 1);

    if(!axisYvalues.isEmpty()) {
        for(int i = 0; i <= tickCountY + 1; ++i) { // ось Y
            QPointF p1(plotArea.x() - maxLabelWidth - DASH_SIZE, plotArea.y() + i * dh);
            painter->drawText(p1, axisYvalues.at(i));
        }
    }

    if(!dxIsNull) { // не равно 0
        startValue = domain->getMinX();
        for(int i = 0; i <= tickCountX + 1; ++i) {   // ось X
            qint64 sec = startValue + i * interval.x();

#if(QT_VERSION > 0x050800)
            QDateTime date = QDateTime::fromSecsSinceEpoch(sec, timeSpec);
#else
            QDateTime date = QDateTime::fromMSecsSinceEpoch(sec * 1000, timeSpec);
#endif

            if(!date.isValid()) continue;
            QString strvalue = date.toString(dateFormat);

            QRect fontRect = getFontMetrix(labelsFont, strvalue);

            qreal px = plotArea.x() + i * dw - fontRect.width() * 0.5;
            if(px < boundingrect.x()) px = boundingrect.x() + DASH_SIZE;
            else if(px > boundingrect.x() + boundingrect.width() - fontRect.width()) px = boundingrect.x() + boundingrect.width() - fontRect.width() - DASH_SIZE;

            QPointF p1(px, plotArea.y() + plotArea.height() + fontRect.height() + DASH_SIZE);

            painter->drawText(p1, strvalue);
        }
    }

    // * koef + indent
    QString corr;
    // Если множитель не равен 1
    if(!qFuzzyCompare(factor, 1)) corr.append(QString::number(factor, 'e', 0) + " ");
    if(!qFuzzyCompare(offset + 1, 1)) {
        qreal temp = offset < 0 ? (-1 * offset) : offset;
        QString str = QString(offset < 0 ? "-" : "+") + " " + QString::number(temp);
        corr.append(str);
    }

    if(!corr.isEmpty()) painter->drawText(QPointF(plotArea.x(), plotArea.y() - DASH_SIZE), corr);


    // Рамка вокруг
    painter->setPen(axisPen);
    painter->drawRect(plotArea);


    painter->setPen(gridPen);

    if(!dxIsNull) { // не равно 0
        // сетка
        for(int i = 1; i <= tickCountX; ++i) {
            QPointF p1(plotArea.x() + i * dw, plotArea.y() + DASH_SIZE);
            QPointF p2(plotArea.x() + i * dw, plotArea.y() + plotArea.height() - DASH_SIZE); //@N 10
            painter->drawLine(p1, p2);
        }
    }

    if(!dyIsNull) { // не равно 0
        for(int i = 1; i <= tickCountY; ++i) {
            QPointF p1(plotArea.x() + DASH_SIZE, plotArea.y() + i * dh);
            QPointF p2(plotArea.x() + plotArea.width() - DASH_SIZE, plotArea.y() + i * dh);
            painter->drawLine(p1, p2);
        }
    }

    // маленькие черточки
    painter->setPen(gridSolidPen);

    if(!dxIsNull) { // не равно 0
        for(int i = 1; i <= tickCountX ; ++i) {
            QPointF p1(plotArea.x() + i * dw, plotArea.y());
            QPointF p2(plotArea.x() + i * dw, plotArea.y() + plotArea.height());

            painter->drawLine(p1, QPointF(p1.x(), p1.y() + DASH_SIZE));
            painter->drawLine(QPointF(p2.x(), p2.y() - DASH_SIZE), p2);
        }
    }

    if(!dyIsNull) { // не равно 0
        for(int i = 1; i <= tickCountY; ++i) {
            QPointF p1(plotArea.x(), plotArea.y() + i * dh);
            QPointF p2(plotArea.x() + plotArea.width(), plotArea.y() + i * dh);
            painter->drawLine(p1, QPointF(p1.x() + DASH_SIZE, p1.y()));
            painter->drawLine(QPointF(p2.x() - DASH_SIZE, p2.y()), p2);
        }
    }
}



