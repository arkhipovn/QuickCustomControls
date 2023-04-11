#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlIndex>

class SqlTableModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(QString tableName READ tableName WRITE setTableName NOTIFY tableNameChanged)

public:
    explicit SqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    virtual ~SqlTableModel() = default;

    Q_INVOKABLE bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                                   int role = Qt::EditRole) override;
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Q_INVOKABLE void clear();
    Q_INVOKABLE void reset();   // полностью сбрасывает модель

    Q_INVOKABLE void sort(int column, Qt::SortOrder order) override;

    void fetchMore(const QModelIndex &parent = QModelIndex()) override;
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const override;

    QSqlDatabase db() const;

    QString tableName() const;
    void setTableName(const QString &tableName);

    QSqlQuery query() const;

    QString filter() const;
    void setFilter(const QString &filter);

    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());

public slots:
    virtual bool select();

signals:
    void tableNameChanged();

protected:
    QSqlDatabase db_;
    QString tableName_;
    QSqlQuery query_;
    int rows_;

    QString filter_;

    int sortColumn_;
    Qt::SortOrder sortOrder_;


    QVector <QVariant> horizontalHeader_;

    QSqlRecord records_;                // Заголовок
    QVector <int> columnOffsets_;       // Если будут добавлены новые колонки, чтобы была возможность связать колонки таблицы и таблицы из бд

    QSqlIndex primaryIndex_;

protected:
    void readRecords();
    void setQuery(const QSqlQuery &query);
    QString createSelect() const;
    QString createSelect(const QSqlRecord &records) const;

    int column(int modelColumn) const;              // Вернет колонку из базы данных с учетом вставленные новых колонок
    QModelIndex columnIndex(const QModelIndex &index) const;



    QString orderBy() const;
    QString update(const QModelIndex &index, const QVariant &var) const;
    QString where() const;
};

#endif // SQLTABLEMODEL_H
