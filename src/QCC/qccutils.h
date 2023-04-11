#ifndef CHARTVIEWUTILS_H
#define CHARTVIEWUTILS_H

#include <QObject>
#include <QQmlEngine>
#include <QTimeZone>
#include <QDebug>

class QCCUtils : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE static void saveChart(const QString &fileName, const QImage &img);
    Q_INVOKABLE static void printChart(const QImage &img, const QString &title = "");

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    // Тк в qml время всегда выводится в LocalTime приходится прибавлять смещение, чтобы отобразить в нужной зоне
    Q_INVOKABLE static QDateTime qmlDateTime(const QDateTime &dt, Qt::TimeSpec timeSpec, int offsetFromUtc = 0);

    // Обратное преобразование. Из времени qml в нужное
    Q_INVOKABLE static QDateTime dateTimeFromQml(const QDateTime &qmlTime, Qt::TimeSpec timeSpec, int offsetFromUtc = 0);

private:
    explicit QCCUtils(QObject *parent = nullptr);
    ~QCCUtils() { qDebug() << "~QCCUtils";}
};


#endif // CHARTVIEWUTILS_H
