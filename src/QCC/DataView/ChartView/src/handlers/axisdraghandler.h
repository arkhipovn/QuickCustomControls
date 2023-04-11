#ifndef AXISDRAGHANDLER_H
#define AXISDRAGHANDLER_H

#include <QObject>
#include <QQuickItem>
#include "../axis/caxis.h"
#include "math.h"
#include <QDebug>

class AxisDragHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enable READ getEnable WRITE setEnable NOTIFY enableChanged)

    Q_PROPERTY(CAxis* target READ getTarget WRITE setTarget NOTIFY targedChanged)
    Q_PROPERTY(Mode mode READ getMode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(int button READ getButton NOTIFY buttonChanged)

    Q_PROPERTY(QPointF mousePress READ getMousePress NOTIFY mousePressChanged)
    Q_PROPERTY(QPointF mouseMove READ getMouseMove NOTIFY mouseMoveChanged)
    Q_PROPERTY(bool pressed READ getPress NOTIFY pressedChanged)

public:
    explicit AxisDragHandler(QObject *parent = nullptr);

    enum Mode {
        Marker = 0,
        Zooming = 1,
        Inaction = 2
    };
    Q_ENUMS(Mode)

    CAxis *getTarget() const;
    void setTarget(CAxis *value);

    AxisDragHandler::Mode getMode() const;
    void setMode(const Mode &value);

    QPointF getMousePress() const;
    QPointF getMouseMove() const;

    bool getPress() const;

    Qt::MouseButton getButton() const;

    bool getEnable() const;
    void setEnable(bool value);

public slots:
    void setValues(qreal minX, qreal maxX, qreal minY, qreal maxY);
    void resetTransform();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void touchEvent(QTouchEvent *event);

    void pressEvent(const QPointF &point, Qt::MouseButton button = Qt::NoButton);
    void moveEvent(const QPointF &point, Qt::MouseButton button = Qt::NoButton);

    void releaseEvent();
    void releaseEvent(const QPointF &point, Qt::MouseButton button = Qt::NoButton);

private:
    bool enable;
    CAxis* target;
    AxisDragHandler::Mode mode;

    Qt::MouseButton button;
    QPointF mousePress;
    QPointF mouseMove;
    bool press;

    void zoom(const QRectF &rect);
    void shiftAxis(int dx, int dy);

    void setPress(bool value);
    void setMousePress(const QPointF &value);
    void setMouseMove(const QPointF &value);
    void setButton(const Qt::MouseButton &value);

signals:
    void move(const QPointF &point, int button);
    void pressed(const QPointF &point, int button);
    void clicked(const QPointF &point, int button);
    void released();

    void pressedChanged();
    void mousePressChanged();
    void mouseMoveChanged();

    void targedChanged();
    void modeChanged();
    void buttonChanged();

    void enableChanged();
};

#endif // AXISDRAGHANDLER_H
