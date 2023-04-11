#ifndef XYMAPPER_H
#define XYMAPPER_H

#include <QObject>
#include <QAbstractItemModel>
#include <QXYSeries>

QT_CHARTS_BEGIN_NAMESPACE

/*
    Отображает QAbstractItemModel в QXYSeries и обновляется при любых изменениях
*/

class XYMapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QXYSeries *series READ series WRITE setSeries NOTIFY seriesChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)

    Q_PROPERTY(int xSection READ xSection WRITE setXSection NOTIFY xSectionChanged)
    Q_PROPERTY(int ySection READ ySection WRITE setYSection NOTIFY ySectionChanged)
    Q_PROPERTY(int first READ first WRITE setFirst NOTIFY firstChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)

    Q_PROPERTY(int seriesNameSection READ seriesNameSection WRITE setSeriesNameSection NOTIFY seriesNameSectionChanged)

public:
    explicit XYMapper(QObject *parent = nullptr);
    ~XYMapper();

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    QXYSeries *series() const;
    void setSeries(QXYSeries *series);

    int xSection() const;
    void setXSection(int xSection);

    int ySection() const;
    void setYSection(int ySection);

    int first() const;
    void setFirst(int first);

    int count() const;
    void setCount(int count);

    int seriesNameSection() const;
    void setSeriesNameSection(int seriesNameSection);

private:
    void blockSeriesSignals(bool b);
    bool seriesSignalBlocked() const;
    qreal valueFromModel(const QModelIndex &index) const;
    int last() const;   // Последния секция, которую нужно взять

    int getModelSize() const; // Длина модели (по умолчанию - это кол-во строк)

    void replace(const QModelIndex &topLeft, const QModelIndex &bottomRight);


private slots:
    void initializeSeriesNameFromModel();
    void initializeDataFromModel();
    void seriesDestroyed();
    void modelDestroyed();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());   //@N orientation
    void rowsInserted(const QModelIndex &parent, int start, int end);   //@N orientation
    void rowsRemoved(const QModelIndex &parent, int start, int end);    //@N orientation
    void columnsInserted(const QModelIndex &parent, int start, int end);    //@N orientation
    void columnsRemoved(const QModelIndex &parent, int start, int end); //@N orientation
    void headerDataChanged(Qt::Orientation orientation, int start, int end);

signals:
    void seriesChanged();
    void modelChanged();

    void xSectionChanged();
    void ySectionChanged();
    void firstChanged();
    void countChanged();

    void seriesNameSectionChanged();

private:
    QAbstractItemModel *model_;
    QXYSeries *series_;

    bool seriesSignalBlocked_;

    Qt::Orientation orientation_; //@N orientation пока что только работает с Qt::Vertical

    int xSection_;
    int ySection_;
    int first_;
    int count_;

    // Если seriesNameSection_ != -1, значит из этой секции нужно брать название для series_
    int seriesNameSection_;
};


QT_CHARTS_END_NAMESPACE

#endif // XYMAPPER_H

