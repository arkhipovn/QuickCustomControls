#ifndef MULTIPLEDATASOURCE_H
#define MULTIPLEDATASOURCE_H

#include <QAbstractTableModel>
#include <CChartView>

class MultipleDataSource : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit MultipleDataSource(QObject *parent = nullptr);
    virtual ~MultipleDataSource() = default;
    QHash<int, QByteArray> roleNames() const;

    virtual int count() const = 0;  // это относится к графику..

public slots:
    virtual void update(int index, CSeries *series) = 0;
    virtual QString getTitle(int index) const = 0;
    //    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

signals:
    void countChanged();    // появились данные или наоборот пропали..
    void sampleChanged(int index = -1);   // -1 - обновили все
    void sampleRemoved(int index = -1);   // -1 - удалили все
};

#endif // MULTIPLEDATASOURCE_H


