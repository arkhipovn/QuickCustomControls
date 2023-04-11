#include "axisdraghandler.h"

AxisDragHandler::AxisDragHandler(QObject *parent) : QObject(parent)
{
    target = nullptr;
    enable = true;
    press = false;
    mode = AxisDragHandler::Zooming;
    button = Qt::NoButton;
    mousePress = QPointF(0, 0);
    mouseMove = QPointF(0, 0);
}

CAxis *AxisDragHandler::getTarget() const
{
    return target;
}

void AxisDragHandler::setTarget(CAxis *value)
{
    target = value;
    if(target == nullptr) return;

#if(QT_VERSION > 0x051000)//@N
    target->setAcceptTouchEvents(true);
#endif

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)

#else
    target->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
#endif

    // перехватываем
    target->installEventFilter(this);
    emit targedChanged();
}

void AxisDragHandler::resetTransform()
{
    if(target == nullptr) return;
    target->getDomain()->resetTransform();
}

bool AxisDragHandler::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    switch (event->type()) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
        case QEvent::TouchCancel:
            touchEvent(static_cast<QTouchEvent*>(event));
            break;
        case QEvent::MouseMove:
            mouseMoveEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonPress:
            mousePressEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            mouseReleaseEvent(static_cast<QMouseEvent*>(event));
            break;
            //#if QT_CONFIG(wheelevent)
        case QEvent::Wheel:
            wheelEvent(static_cast<QWheelEvent*>(event));
            break;
            //#endif
        default:
            QObject::event(event);
            break;
    }

    return true;
}

void AxisDragHandler::mouseMoveEvent(QMouseEvent *event)
{
    moveEvent(event->pos(), button);
}

void AxisDragHandler::mousePressEvent(QMouseEvent *event)
{
    if(!enable) return;
    if(press) return;
    setButton(event->button());
    pressEvent(event->pos(), event->button());
}

void AxisDragHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if(!press) return;
    setButton(Qt::NoButton);
    releaseEvent(event->pos(), event->button());
}

void AxisDragHandler::wheelEvent(QWheelEvent *event)
{
    if(target == nullptr) return;
    if(target->getType() == CAxis::LogValueAxisType) return;
    if(event->angleDelta().y() > 0) target->getDomain()->zoomIn();
    else target->getDomain()->zoomOut();
}
/*
void AxisDragHandler::touchEvent(QTouchEvent *event)
{
    auto d = target->getDomain();
    auto plotArea = target->boundingRect();//getPlotArea();

    static int startDist = 0;
    static bool zooming = false;

    QList <QTouchEvent::TouchPoint> points = event->touchPoints();

    QPointF centroid(plotArea.width() / 2., plotArea.height() / 2.);

    QPointF presscentroid;
    for(auto p : points) {
        presscentroid += p.pos();
    }
    presscentroid /= points.size();

    emit tempsignal(presscentroid, centroid);



    // points.first().pos()
    switch (event->type()) {
    case QTouchEvent::TouchBegin: {
        break;
    }
    case QEvent::TouchUpdate: {
        if(points.size() == 2) {
            if(!zooming) {
                startDist = 0;
                zooming = true;
                return;
            }


        }
        break;
    }
    case QTouchEvent::TouchEnd: {
        break;
    }
    default: break;
    }
}*/


//void AxisDragHandler::touchEvent(QTouchEvent *event)
//{
//    auto d = target->getDomain();
//    auto plotArea = target->boundingRect();//getPlotArea();

//    static bool zooming = false;
//    //    static QVector2D startDist = QVector2D(0, 0);
//    //    static QVector2D accumulatedScale = QVector2D(d->getScaleX(), d->getScaleY());

//    static qreal startDist = 0;
//    static qreal accumulatedScaleX = d->getScaleX();
//    static qreal accumulatedScaleY = d->getScaleY();


//    QList <QTouchEvent::TouchPoint> points = event->touchPoints();


//    //@N тычок..

//    QPointF centroid((plotArea.width() - plotArea.x()) / 2, (plotArea.height() - plotArea.y()) / 2);

//    float dist = 0; //@N
//    //    QVector2D currentDist;
//    for(auto p : points) {
//        //        currentDist += QVector2D(p.pos() - centroid);
//        dist += QVector2D(p.pos() - centroid).length(); //@N
//    }
//    dist /= points.size(); //@N

//    switch (event->type()) {
//    case QTouchEvent::TouchBegin: {
//        switch (points.size()) {
//        case 1: pressEvent(points.first().pos()); break;
//        case 2: break;
//        default: break;
//        }
//        break;
//    }
//    case QEvent::TouchUpdate: {
//        switch (points.size()) {
//        case 1: {
//            if(zooming) return;
//            moveEvent(points.first().pos());
//            break;
//        }
//        case 2: {
//            if(!zooming) {
//                accumulatedScaleX = d->getScaleX();
//                accumulatedScaleY = d->getScaleY();
//                startDist = dist;
//                zooming = true;
//                break;
//            }

//            QVector2D v2(points.first().pos() - points.last().pos());
//            qreal angle = acos(v2.x() / (1 + v2.length())) * 180 / 3.1415926;
//            if(angle > 90) angle = 180 - angle;

//            qreal activeScaleX = dist * accumulatedScaleX / startDist;
//            qreal activeScaleY = dist * accumulatedScaleY / startDist;
//            if(angle > 45) d->setScale(accumulatedScaleX, activeScaleY);
//            else d->setScale(activeScaleX, accumulatedScaleY);

//            emit tempsignal(QPointF(0, 0), QPointF(0, 0), angle);

//            break;
//        }
//        default: break;
//        }
//        break;
//    }
//    case QTouchEvent::TouchEnd: {
//        zooming = false;
//        switch (points.size()) {
//        case 1:
//            releaseEvent(points.first().pos());
//            break;
//        case 2: break;
//        default:
//            releaseEvent();
//            break;
//        }
//        break;
//    }
//    default: break;
//    }
//}


void AxisDragHandler::touchEvent(QTouchEvent *event)
{
    auto d = target->getDomain();
    auto plotArea = target->boundingRect();//getPlotArea();

    static bool zooming = false;
    //    static QVector2D startDist = QVector2D(0, 0);
    //    static QVector2D accumulatedScale = QVector2D(d->getScaleX(), d->getScaleY());

    static QVector2D startDist(0, 0);
    static QVector2D accumulatedScale;

    QList <QTouchEvent::TouchPoint> points = event->touchPoints();

    switch (event->type()) {
        case QTouchEvent::TouchBegin: {
            if(!enable) return;
            switch (points.size()) {
                case 1: pressEvent(points.first().pos()); break;
                case 2: break;
                default: break;
            }
            break;
        }
        case QEvent::TouchUpdate: {
            switch (points.size()) {
                case 1: {
                    if(zooming) return;
                    moveEvent(points.first().pos());
                    break;
                }
                case 2: {
                    QPointF centroid((plotArea.width() - plotArea.x()) / 2, (plotArea.height() - plotArea.y()) / 2);
                    QVector2D currentDist(qAbs(points.first().pos().x() - points.last().pos().x()), qAbs(points.first().pos().y() - points.last().pos().y()));

                    if(!zooming) {
                        startDist = currentDist;
                        accumulatedScale = QVector2D(d->getScaleX(), d->getScaleY());
                        zooming = true;
                        break;
                    }

                    QVector2D v2(points.first().pos() - points.last().pos());
                    qreal angle = acos(v2.x() / (1 + v2.length())) * 180 / 3.1415926;
                    if(angle > 90) angle = 180 - angle;

                    QVector2D activeScale = currentDist * accumulatedScale / startDist;

                    if(angle < 30) d->setScale(activeScale.x(), d->getScaleY());
                    else if(angle > 60) d->setScale(d->getScaleX(), activeScale.y());
                    else d->setScale(activeScale.x(), activeScale.y());

                    break;
                }
                default: break;
            }
            break;
        }
        case QTouchEvent::TouchEnd: {
            zooming = false;
            switch (points.size()) {
                case 1:
                    releaseEvent(points.first().pos());
                    break;
                case 2: break;
                default:
                    releaseEvent();
                    break;
            }
            break;
        }
        default: break;
    }
}

void AxisDragHandler::pressEvent(const QPointF &point, Qt::MouseButton button)
{
    if(!target->getPlotArea().contains(point)) return;

    emit pressed(point, button);

    setPress(true);
    setMousePress(point);
    setMouseMove(point);

    switch (mode) {
        case Marker: break;
        case Zooming:
            switch (button) {
                case Qt::NoButton:
                case Qt::LeftButton:
                    // если нет кнопок или левая, то двигаем
                    break;
                case Qt::RightButton:
                    // масштабируем
                    break;
                default: break;

            }
        default: break;
    }
}

void AxisDragHandler::moveEvent(const QPointF &point, Qt::MouseButton button)
{
    if(!press) return;
    if(!target->getPlotArea().contains(point)) return;
    emit move(point, button);

    // защита
    if(sqrt((point - mousePress).manhattanLength()) < 3) return;

    switch (mode) {
        case Marker: break;
        case Zooming:
            if(target->getType() == CAxis::LogValueAxisType) break;
            switch (button) {
                case Qt::NoButton:
                case Qt::LeftButton: {
                    // если нет кнопок или левая, то двигаем
                    qreal dx = point.x() - mouseMove.x();
                    qreal dy = point.y() - mouseMove.y();
                    shiftAxis(dx, dy);
                    break;
                }
                case Qt::RightButton:
                    // масштабируем
                    break;
                default: break;

            }
        default: break;
    }

    setMouseMove(point);
}

void AxisDragHandler::releaseEvent()
{
    emit released();
    setPress(false);
}

void AxisDragHandler::releaseEvent(const QPointF &point, Qt::MouseButton button)
{
    if(mousePress == point) emit clicked(point, button);

    //    if(!target->getPlotArea().contains(point)) return; //@N задать границу...
    switch (mode) {
        case Marker: break;
        case Zooming:
            if(target->getType() == CAxis::LogValueAxisType) break;
            switch (button) {
                case Qt::NoButton:
                case Qt::LeftButton:
                    // если нет кнопок или левая, то двигаем
                    break;
                case Qt::RightButton:
                    // масштабируем
                    zoom(QRectF(mousePress, point));
                    break;
                default: break;

            }
        default: break;
    }

    AxisDragHandler::releaseEvent();
}

bool AxisDragHandler::getEnable() const
{
    return enable;
}

void AxisDragHandler::setEnable(bool value)
{
    if(enable != value) {
        enable = value;
        emit enableChanged();
    }
}

void AxisDragHandler::zoom(const QRectF &rect)
{
    if(target == nullptr) return;
    if(qAbs(rect.width()) < 1 && qAbs(rect.height()) < 1) return;
    auto d = target->getDomain();
    
    QRectF fixedRect = rect;
    if(rect.width() < 0) {
        fixedRect.setX(rect.x() + rect.width());
        fixedRect.setWidth(-rect.width());
    }
    if(rect.height() < 0) {
        fixedRect.setY(rect.y() + rect.height());
        fixedRect.setHeight(-rect.height());
    }

    auto plotArea = target->getPlotArea();
    fixedRect.setX(fixedRect.x() - plotArea.x());
    fixedRect.setY(fixedRect.y() - plotArea.y());
    fixedRect.setWidth(fixedRect.width() - plotArea.x());
    fixedRect.setHeight(fixedRect.height() - plotArea.y());

    qreal spanX = d->getMaxX() - d->getMinX();
    qreal spanY = d->getMaxY() - d->getMinY();

    qreal dx = spanX / target->getPlotArea().width();
    qreal dy = spanY / target->getPlotArea().height();

    qreal maxX = d->getMinX() + dx * fixedRect.right();
    qreal minX = d->getMinX() + dx * fixedRect.left();
    qreal minY = d->getMaxY() - dy * fixedRect.bottom();
    qreal maxY = d->getMaxY() - dy * fixedRect.top();

    //@N? нужно
    if ((maxX - minX) == spanX) {
        minX = d->getMinX();
        maxX = d->getMaxX() ;
    }
    if ((maxY - minY) == spanY) {
        minY = d->getMinY();
        maxY = d->getMaxY();
    }

    d->zoom(minX, maxX, minY, maxY);
}

void AxisDragHandler::shiftAxis(int dx, int dy)
{
    if(target == nullptr) return;
    auto d = target->getDomain();

    qreal spanX = d->getMaxX() - d->getMinX();
    qreal spanY = d->getMaxY() - d->getMinY();
    qreal x = spanX / target->getPlotArea().width();
    qreal y = spanY / target->getPlotArea().height();

    d->move(x * dx, y * dy);
}

AxisDragHandler::Mode AxisDragHandler::getMode() const { return mode; }
QPointF AxisDragHandler::getMouseMove() const { return mouseMove; }
QPointF AxisDragHandler::getMousePress() const { return mousePress; }
bool AxisDragHandler::getPress() const { return press; }
Qt::MouseButton AxisDragHandler::getButton() const { return button; }

void AxisDragHandler::setValues(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    if(target == nullptr) return;
    resetTransform();
    auto d = target->getDomain();
    d->setRange(minX, maxX, minY, maxY);
}

void AxisDragHandler::setMode(const AxisDragHandler::Mode &value)
{
    if(mode != value) {
        mode = value;
        emit modeChanged();
        if(target == nullptr) return;
        if(mode == AxisDragHandler::Inaction)
            target->setEnabled(false);
        else
            target->setEnabled(true);
    }
}

void AxisDragHandler::setPress(bool value)
{
    if(press != value) {
        press = value;
        emit pressedChanged();
    }
}

void AxisDragHandler::setMouseMove(const QPointF &value)
{
    if(mouseMove != value) {
        mouseMove = value;
        emit mouseMoveChanged();
    }
}

void AxisDragHandler::setButton(const Qt::MouseButton &value)
{
    if(button != value) {
        button = value;
        emit buttonChanged();
    }
}

void AxisDragHandler::setMousePress(const QPointF &value)
{
    if(mousePress != value) {
        mousePress = value;
        emit mousePressChanged();
    }
}
