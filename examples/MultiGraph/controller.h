#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QVariantList>
//#include "multidatasource.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);

public slots:
    void drawBigData(const QString &title = "");
    void drawSmallData(const QString &title = "");

//    QObject *getSource() const;

private:
//    MultiDataSource *source;
};

#endif // CONTROLLER_H
