#ifndef WAITINGBAR_H
#define WAITINGBAR_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QTimerEvent>
#include <QEasingCurve>
#include <QTimer>

// переделать через QQuickItem..

class WaitingBar : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int duration READ getDuration WRITE setDuration NOTIFY durationChanged)
    //    Q_PROPERTY(qreal progressBarWidth READ getProgressBarWidth WRITE setProgressBarWidth NOTIFY progressBarWidthChanged)
    Q_PROPERTY(qreal progressBarWidth READ getProgressBarWidth NOTIFY progressBarWidthChanged)

    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor progressColor READ getProgressColor WRITE setProgressColor NOTIFY progressColorChanged)

public:
    WaitingBar(QQuickItem *parent = nullptr);
    ~WaitingBar();

    bool getRunning() const;
    void setRunning(bool value);

    int getDuration() const;
    void setDuration(int value);

    qreal getProgressBarWidth() const;
    void setProgressBarWidth(qreal value);

    QColor getColor() const;
    void setColor(const QColor &value);

    QColor getProgressColor() const;
    void setProgressColor(const QColor &value);

    void setVisible(bool value);

public slots:
    void start();
    void stop();

protected:
//    void componentComplete() override;
    void paint(QPainter *painter);
    void geometryChanged(const QRectF &newGeometry,
                         const QRectF &oldGeometry);

private slots:
    void timeout();

signals:
    void runningChanged();
    void durationChanged();
    void progressBarWidthChanged();

    void colorChanged();
    void progressColorChanged();

private:
    QColor color;
    QColor progressColor;


    QEasingCurve easingCurve;
    qreal interval;
    qreal progress;
    int duration;

    qreal progressBarWidth;

    QTimer *timer;
};

#endif // WAITINGBAR_H

