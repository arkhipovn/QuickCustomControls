#ifndef VCHICON_H
#define VCHICON_H

#include <QQuickPaintedItem>
#include <QDateTime>
#include <QTimer>

class VchIcon : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QDateTime dateTime READ getDateTime WRITE setDateTime NOTIFY dateTimeChanged)  // мб не стоит сигнал отправлять каждый раз?
    Q_PROPERTY(Qt::TimeSpec timeSpec READ getTimeSpec WRITE setTimeSpec NOTIFY timeSpecChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool running READ isActive WRITE setActive NOTIFY activeChanged)

public:
    VchIcon(QQuickItem *parent = 0);

    Qt::TimeSpec getTimeSpec() const;
    void setTimeSpec(const Qt::TimeSpec &value);

    int interval() const;   // msec
    void setInterval(int msec);

    bool isActive() const;
    void setActive(bool active);

    QDateTime getDateTime() const;
    void setDateTime(const QDateTime &value);

public slots:
    void start();
    void stop();

protected:
    void paint(QPainter *painter) override;

signals:
    void dateTimeChanged(); // пока что не отправляется..
    void activeChanged();
    void intervalChanged();
    void timeSpecChanged();

private slots:
    void timeout();

private:
    QTimer timer;

    Qt::TimeSpec timeSpec;
    QDateTime datetime;

    QPolygonF getPoligon(const QRectF &rec, qreal temp1 = 0.1235) const;
};

#endif // VCHICON_H

