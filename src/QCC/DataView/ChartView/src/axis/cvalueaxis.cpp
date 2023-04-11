#include "cvalueaxis.h"

CValueAxis::CValueAxis(QQuickItem *parent) : CAxis(ValueAxisType, parent), offset(0),
    factor(1), interval(QPointF(0, 0)), precision(1), offsetActive(true), factorActive(true)
{
    domain = new CXYDomain(this);
    QObject::connect(domain, &CDomain::updated, this, [this]() {
        updateAxisSettings();
        update();
        emit axisRangeChanged();
    });

    QObject::connect(domain, &CDomain::rangeVerticalChanged, this, &CAxis::rangeVerticalChanged);
    QObject::connect(domain, &CDomain::rangeHorizontalChanged, this, &CAxis::rangeHorizontalChanged);

    updateAxisSettings();
}

CValueAxis::CValueAxis(CAxis::TypeAxis type, QQuickItem *parent) : CAxis(type, parent), offset(0),
    factor(1), interval(QPointF(0, 0)), precision(1), offsetActive(true), factorActive(true)
{
    domain = new CXYDomain(this);
    QObject::connect(domain, &CDomain::updated, this, [this]() {
        updateAxisSettings();
        update();
        emit axisRangeChanged();
    });

    QObject::connect(domain, &CDomain::rangeVerticalChanged, this, &CAxis::rangeVerticalChanged);
    QObject::connect(domain, &CDomain::rangeHorizontalChanged, this, &CAxis::rangeHorizontalChanged);

    updateAxisSettings();
}

QPointF CValueAxis::mapToPosition(const QPointF &value)
{
    qreal nx = (value.x() - domain->getMinX()) / (domain->getMaxX() -  domain->getMinX());
    qreal ny = 1 - (value.y() - domain->getMinY()) / (domain->getMaxY() -  domain->getMinY());
    qreal x = plotArea.x() + nx * plotArea.width();
    qreal y = plotArea.y() + ny * plotArea.height();
    return QPointF(x, y);
}

QPointF CValueAxis::mapToValue(const QPointF &point)
{
    qreal nx = (point.x() - plotArea.x()) / plotArea.width();
    qreal ny = 1 - (point.y() - plotArea.y()) / plotArea.height();
    return QPointF(nx * (domain->getMaxX() - domain->getMinX()) + domain->getMinX(), ny * (domain->getMaxY() - domain->getMinY()) + domain->getMinY());
}

void CValueAxis::paint(QPainter *painter)
{
    painter->setFont(labelsFont);

    //    updateAxisSettings(); // @N перенести на изменение мин, макс

    QRectF boundingrect = boundingRect();

    // Текст
    painter->setPen(labelsPen);

    // расчитаем высоту и ширину текста по оси Y
    int maxLabelHeight = 0;
    int maxLabelWidth = 0;
    QStringList axisYvalues;
    qreal startValue = (domain->getMinY() - offset) / factor;
    for(int i = 0; i <= tickCountY + 1; ++i){
        qreal value = startValue + (tickCountY + 1 - i) * interval.y();
        QString strvalue = factorActive ? QString::number(value, 'f', precision)
                                        : QString::number(value, 'g', 6);
        axisYvalues.append(strvalue);
        QRect fontRect = getFontMetrix(labelsFont, strvalue);
        if(maxLabelWidth < fontRect.width()) maxLabelWidth = fontRect.width();
        if(maxLabelHeight < fontRect.height()) maxLabelHeight = fontRect.height();
    }

    QMarginsF margins(maxLabelWidth + 2 * DASH_SIZE, maxLabelHeight * 2, maxLabelWidth * 0.5, maxLabelHeight * 2);
    setPlotArea(boundingrect.marginsRemoved(margins));

    qreal dw = plotArea.width() / static_cast<qreal>(tickCountX + 1);
    qreal dh = plotArea.height() / static_cast<qreal>(tickCountY + 1);

    for(int i = 0; i <= tickCountY + 1; ++i) { // ось Y
        QPointF p1(plotArea.x() - maxLabelWidth - DASH_SIZE, plotArea.y() + i * dh);
        painter->drawText(p1, axisYvalues.at(i));
    }


    startValue = domain->getMinX();
    for(int i = 0; i <= tickCountX + 1; ++i) {   // ось X
        QString strvalue = QString::number(startValue + i * interval.x(), 'g', 5);

        QRect fontRect = getFontMetrix(labelsFont, strvalue);

        qreal px = plotArea.x() + i * dw - fontRect.width() * 0.5;
        if(px < boundingrect.x()) px = boundingrect.x() + DASH_SIZE;
        else if(px > boundingrect.x() + boundingrect.width() - fontRect.width()) px = boundingrect.x() + boundingrect.width() - fontRect.width() - DASH_SIZE;
        QPointF p1(px, plotArea.y() + plotArea.height() + fontRect.height() + DASH_SIZE);
        painter->drawText(p1, strvalue);
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

    // сетка
    for(int i = 1; i <= tickCountX; ++i) {
        QPointF p1(plotArea.x() + i * dw, plotArea.y() + DASH_SIZE);
        QPointF p2(plotArea.x() + i * dw, plotArea.y() + plotArea.height() - DASH_SIZE);
        painter->drawLine(p1, p2);
    }

    for(int i = 1; i <= tickCountY; ++i) {
        QPointF p1(plotArea.x() + DASH_SIZE, plotArea.y() + i * dh);
        QPointF p2(plotArea.x() + plotArea.width() - DASH_SIZE, plotArea.y() + i * dh);
        painter->drawLine(p1, p2);
    }


    // маленькие черточки
    painter->setPen(gridSolidPen);
    for(int i = 1; i <= tickCountX ; ++i) {
        QPointF p1(plotArea.x() + i * dw, plotArea.y());
        QPointF p2(plotArea.x() + i * dw, plotArea.y() + plotArea.height());

        painter->drawLine(p1, QPointF(p1.x(), p1.y() + DASH_SIZE));
        painter->drawLine(QPointF(p2.x(), p2.y() - DASH_SIZE), p2);
    }

    for(int i = 1; i <= tickCountY; ++i) {
        QPointF p1(plotArea.x(), plotArea.y() + i * dh);
        QPointF p2(plotArea.x() + plotArea.width(), plotArea.y() + i * dh);
        painter->drawLine(p1, QPointF(p1.x() + DASH_SIZE, p1.y()));
        painter->drawLine(QPointF(p2.x() - DASH_SIZE, p2.y()), p2);
    }
}

inline int getCountsOfDigits(unsigned long long number)
{
    return (number == 0) ? 1 : (static_cast<int>(ceil(log10(qAbs(number)) + 0.5)));
}

qreal calcFactor(qreal value)
{
    qreal factor = 1;
    int countOfDigits = 1;
    if(value < 0) value *= -1; // сделаем положительным..
    if(value < 1){
        countOfDigits = -1;
        value = 1. / value;
    }
    else factor = 0.1;    // нужно на 1 знак уменьшить. Иначе 4 -> 0.4*1e1 получим.. а нужно 4
    countOfDigits *= getCountsOfDigits(value);
    factor *= pow(10, countOfDigits);
    return factor;
}

void CValueAxis::updateAxisSettings()
{
    qreal x = domain->getMaxX() - domain->getMinX();
    interval.setX(x / static_cast<qreal>(tickCountX + 1));

    qreal dy = domain->getMaxY() - domain->getMinY();

    // не больше трех знаков..
    if(qAbs(domain->getMaxY()) > 1000 || qAbs(domain->getMaxY()) < 0.01) {
        if(factorActive) factor = calcFactor(dy);
        else factor = 1;

        if(offsetActive && factorActive) {  //@N && factorActive
            qreal minYfactor = calcFactor(domain->getMinY());
            if(minYfactor / factor > 10) offset = floor(domain->getMinY() / factor) * factor;
            else offset = 0;
        }
        else offset = 0;

        dy /= (qreal)factor;
    }
    else {
        factor = 1;
        offset = 0;
    }

    qreal dY = dy / static_cast<qreal>(tickCountY + 1); //qRound(dy) / (qreal) tickCountY;
    interval.setY(dY);

    if(dY > 100) precision = 0;
    else if(dY > 10) precision = 1;
    else if(dY < 0.1) precision = 3;
    else if(dY < 1) precision = 2;
    else precision = 1;
}

bool CValueAxis::getFactorActive() const
{
    return factorActive;
}

void CValueAxis::setFactorActive(bool value)
{
    if(factorActive != value) {
        factorActive = value;
        updateAxisSettings();
        emit factorActiveChanged();
    }
}

bool CValueAxis::getOffsetActive() const
{
    return offsetActive;
}

void CValueAxis::setOffsetActive(bool value)
{
    if(offsetActive != value) {
        offsetActive = value;
        updateAxisSettings();
        emit factorActiveChanged();
    }
}







