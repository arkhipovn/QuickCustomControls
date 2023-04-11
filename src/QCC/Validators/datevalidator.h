#ifndef DATEVALIDATOR
#define DATEVALIDATOR

#include <QDateTime>
#include <QValidator>
#include <QRegExp>
#include <QDebug>

class DateValidator : public QValidator
{
    Q_OBJECT

    Q_PROPERTY(QString format READ getFormat WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(QString inputMask READ getInputMask NOTIFY formatChanged)

public :
    explicit DateValidator(QObject * const parent = Q_NULLPTR)
        : QValidator{parent} {}


    State validate(QString &input, int &pos) const override {
        Q_UNUSED(pos);

        if(input.contains(regExp)) {
            // вроде норм записано. Проверим нормальная ли дата..
            auto d = QDateTime::fromString(input, format);
            //            qDebug() << "ok" << d << d.isValid();
            return d.isValid() ? QValidator::Acceptable : QValidator::Intermediate;
        }
        else {
            // что-то не то..
            //@N не работает..
            if(format.size() != input.size()) return QValidator::Intermediate;
            for(int i = 0; i < input.size(); ++i) {
                if(input.at(i).isDigit()) continue;
                if(input.at(i) == format.at(i)) continue;
                input[i] = format.at(i);
            }
        }
        //        qDebug() << input;
        return QValidator::Intermediate;
    }

    QString getFormat() const { return format; }
    void setFormat(const QString &value) {
        if(format != value) {

            QString regExpStr = value;
            regExpStr.replace(QRegExp("(d|M|y|h|H|m|s|z)"), "\\d");

            inputMask = value;
            inputMask.replace(QRegExp("(d|M|y|h|H|m|s|z)"), "N"); inputMask.append(";_");

            format = value;
            regExp = QRegExp(regExpStr);
            emit formatChanged();
        }
    }

    QString getInputMask() const { return inputMask; }

public slots:
    QString up(const QString &input, int pos) { return add(input, pos); }
    QString down(const QString &input, int pos) { return add(input, pos, -1); }

signals:
    void formatChanged();

private:
    QString format = "dd.MM.yyyy hh:mm:ss";
    QRegExp regExp = QRegExp("(\\d{2}).(\\d{2}).(\\d{4}) (\\d{2}):(\\d{2}):(\\d{2})");
    QString inputMask = "NN.NN.NNNN NN:NN:NN;_";

    QString add(const QString &input, int pos, int count = 1) {
        if(pos > format.size() || pos < 0 || format.isEmpty()) return input;

        auto date = QDateTime::fromString(input, format);
        if(!date.isValid()) return input;

        if(pos == format.size()) pos = format.size() - 1;
        auto symbol = format.at(pos);

        if(symbol == 'd') date = date.addDays(count);
        else if(symbol == 'M') date = date.addMonths(count);
        else if(symbol == 'y') date = date.addYears(count);
        else if(symbol == 'h' || symbol == 'H') date = date.addSecs(count * 3600);
        else if(symbol == 'm') date = date.addSecs(count * 60);
        else if(symbol == 's') date = date.addSecs(count);

        return date.toString(format);
    }
};

#endif // DATEVALIDATOR

