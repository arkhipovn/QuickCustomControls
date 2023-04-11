#include "controller.h"
#include <QDebug>

Controller::Controller(QObject *parent) : QObject(parent)
{

}

void Controller::drawBigData()
{
    //    double min = 1e-12;
    //    double max = 9e-9;
    //    QList <QPointF> list;
    //    int s = 100000;
    //    for(int i = 1594196524; i < 1594196524 + s; ++i) {
    //        double value = (qrand() / (double)RAND_MAX) * (max - min) + min;
    //        list.append(QPointF(i, value));
    //    }

    QList <QPointF> list;
    double U;
    for(int t = 0; t < 1000000; ++t) {
        U = (static_cast<double>(qrand()) / RAND_MAX) * 5;
        list.append(QPointF(t, U));
    }

    emit drawData(list);
}

void Controller::drawSmallData()
{
    double min = 0;
    double max = 1000000;

    //    double min = 0.000001;
    //    double max = 0.000004;
    //    double min = 1e7;
    //    double max = 1e2;
    QList <QPointF> list;

    //    list.append(QPointF(1, 0));
    //    list.append(QPointF(2, 0));
    //    list.append(QPointF(3, 0));
    //    list.append(QPointF(4, 0));
    //    list.append(QPointF(5, 0));
    //    list.append(QPointF(10, 0));

    //        list.append(QPointF(2, 2));
    //        list.append(QPointF(4, 3));
    //        list.append(QPointF(6, 5));
    //        list.append(QPointF(10, 2));

    int s = 200;
    for(int i = QDateTime::currentSecsSinceEpoch(); i < QDateTime::currentSecsSinceEpoch() + s; ++i) {
        double value = (qrand() / (double)RAND_MAX) * (max - min) + min;
        list.append(QPointF(i, value));
    }
    emit drawData(list);
}

void Controller::drawAdev()
{
    QList <QPointF> list;
    list.append(QPointF(1, 4.9e-13));
    list.append(QPointF(10, 1.01e-13));
    list.append(QPointF(100, 4.07e-14));
    list.append(QPointF(1000, 1.45e-14));
    list.append(QPointF(3600, 7.62e-15));
    list.append(QPointF(10000, 4.69e-15));
    list.append(QPointF(86400, 4.01e-13));
    list.append(QPointF(100000, 3.88e-13));
    emit drawData(list);
}
