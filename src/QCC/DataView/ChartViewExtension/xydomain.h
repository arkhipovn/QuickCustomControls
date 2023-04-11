#ifndef XYDOMAIN_H
#define XYDOMAIN_H

#include <QObject>

class XYDomain : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal maxY READ maxY NOTIFY rangeVerticalChanged)
    Q_PROPERTY(qreal minY READ minY NOTIFY rangeVerticalChanged)
    Q_PROPERTY(qreal minX READ minX NOTIFY rangeHorizontalChanged)
    Q_PROPERTY(qreal maxX READ maxX NOTIFY rangeHorizontalChanged)

public:
    explicit XYDomain(QObject *parent = nullptr);
    virtual ~XYDomain() = default;

    void setRange(Qt::Orientation orientation, qreal min, qreal max);

    void setRangeHorizontal(qreal min, qreal max);  // Тоже самое, что и setRangeX
    void setRangeVertical(qreal min, qreal max);    // Тоже самое, что и setRangeY

    void setRangeX(qreal min, qreal max);
    void setRangeY(qreal min, qreal max);
    void setRange(qreal minX, qreal maxX, qreal minY, qreal maxY);

    qreal minX() const;
    void setMinX(const qreal &minX);

    qreal maxX() const;
    void setMaxX(const qreal &maxX);

    qreal minY() const;
    void setMinY(const qreal &minY);

    qreal maxY() const;
    void setMaxY(const qreal &maxY);

    void reset();

signals:
    void updated();
    void rangeHorizontalChanged();
    void rangeVerticalChanged();

protected:
    qreal minX_;
    qreal maxX_;
    qreal minY_;
    qreal maxY_;
};

#endif // XYDOMAIN_H
