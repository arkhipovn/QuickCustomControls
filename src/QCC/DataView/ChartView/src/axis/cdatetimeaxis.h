#ifndef CDATETIMEAXIS_H
#define CDATETIMEAXIS_H

#include <QDateTime>
#include "cvalueaxis.h"

// передавать в секундах...
class CDateTimeAxis : public CValueAxis
{
    Q_OBJECT

    Q_PROPERTY(QString dateFormat READ getDateFormat WRITE setDateFormat NOTIFY dateFormatChanged)
    Q_PROPERTY(QString formatY READ getDateFormat WRITE setDateFormat NOTIFY dateFormatChanged)         // Осталось для поддержания устаревшего кода
    Q_PROPERTY(Qt::TimeSpec timeSpec READ getTimeSpec WRITE setTimeSpec NOTIFY timeSpecChanged)

public:
    explicit CDateTimeAxis(QQuickItem *parent = 0);

    Qt::TimeSpec getTimeSpec() const;
    void setTimeSpec(Qt::TimeSpec timespec);

    QString getDateFormat() const;
    void setDateFormat(const QString &value);

public slots:
    QString secsToString(qint64 secs, const QString &format = "dd.MM.yyyy hh:mm:ss") const;

protected:
    void paint(QPainter *painter) override;

signals:
    void timeSpecChanged();
    void dateFormatChanged();

private:
    Qt::TimeSpec timeSpec;
    QString dateFormat;
};

#endif // CDATETIMEAXIS_H

