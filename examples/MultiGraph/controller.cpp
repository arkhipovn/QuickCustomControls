#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    //    source = new MultiDataSource;
}

void Controller::drawBigData(const QString &title)
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
    for(int t = 0; t < 86400; ++t) {
        U = (static_cast<double>(qrand()) / RAND_MAX) * 5;
        list.append(QPointF(t, U));
    }

}


void Controller::drawSmallData(const QString &title)
{
    static int number = 1;
    static bool temp = false;
    double min = temp ? -0.01 : -0.001;
    double max = 0.04;

    temp = !temp;

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
    for(int i = 1594196524; i < 1594196524 + s; ++i) {
        double value = (qrand() / (double)RAND_MAX) * (max - min) + min;
        list.append(QPointF(i, value));
    }

    //    int minX = std::numeric_limits<int>::max(); // everything is <= this
    //    int maxX = std::numeric_limits<int>::min(); // everything is >= this
    //    int minY = std::numeric_limits<int>::max();
    //    int maxY = std::numeric_limits<int>::min();
    //    for(auto p : list) {
    //        minX = qMin(minX, p.x());
    //        maxX = qMax(maxX, p.x());
    //        minY = qMin(minY, p.y());
    //        maxY = qMax(maxY, p.y());
    //    }

    //        emit drawData(list/*, minX, maxX, minY, maxY*/);

    //    source->append("line " + QString::number(number), list);
    ++number;
}

//QObject *Controller::getSource() const
//{
//    return source;
//}
