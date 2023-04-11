#ifndef HEXVALIDATOR_H
#define HEXVALIDATOR_H

#include <QDateTime>
#include <QValidator>
#include <QDebug>

class HexValidator : public QValidator
{
    Q_OBJECT

    Q_PROPERTY(int top WRITE setTop READ getTop NOTIFY topChanged)
    Q_PROPERTY(int bottom WRITE setBottom READ getBottom NOTIFY bottomChanged)

public :
    explicit HexValidator( QObject * const parent = Q_NULLPTR)
        : QValidator{parent} {
        this->bottom = -1;
        this->top = -1;
    }

    State validate(QString & input, int & pos) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(pos);

        if (input.isEmpty())
            return Intermediate;

        if(input.compare("0x", Qt::CaseInsensitive) == 0) return Intermediate;

        bool isok;
        int num = input.toInt(&isok, 16);
        if(isok) {
            if(bottom != -1 && num < bottom) return Intermediate;
            if(top != -1 && num > top) return Invalid;
            return Acceptable;
        }
        return Invalid;
    }

    int getTop() const { return top; }
    void setTop(int v) {
        if(v != top) {
            top = v;
            emit topChanged();
        }
    }

    int getBottom() const { return bottom; }
    void setBottom(int v) {
        if(v != bottom) {
            bottom = v;
            emit bottomChanged();
        }
    }

signals:
    void bottomChanged();
    void topChanged();

private:
    int bottom;
    int top;
};

#endif // HEXVALIDATOR_H
