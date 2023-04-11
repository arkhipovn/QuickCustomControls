#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QVariantList>
#include <QDateTime>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);

public slots:
    void drawBigData();
    void drawSmallData();
    void drawAdev();

signals:
    void drawData(QList <QPointF> samples);
};

#endif // CONTROLLER_H
