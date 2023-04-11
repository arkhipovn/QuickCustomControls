#ifndef TABLEFILTERLISTMODEL_H
#define TABLEFILTERLISTMODEL_H

#include <QAbstractListModel>

class TableFilterListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(TableFilterListModel::TypeCondition typeCondition READ typeCondition WRITE setTypeCondition NOTIFY typeConditionChanged)

public:
    explicit TableFilterListModel(QObject *parent = nullptr);
    virtual ~TableFilterListModel() = default;

    QHash <int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &index = QModelIndex()) const;

    enum Role {
        FilterColumn,
        FilterRole,
        FilterId,
        FilterValue,
        FilterNegation
    };

    enum class TypeCondition {
        And,
        Or
    };
    Q_ENUMS(TypeCondition)

    struct FilterIndex
    {
        int column = 0;
        int role = Qt::DisplayRole;
        bool operator ==(const FilterIndex &index) const;
    };

    struct Filter {
        int id = 0;                 // Просто какой-то идентификатор для отображения
        QVariant value;
        bool negation = false;
    };

    using FilterInfo = QPair <FilterIndex, Filter>;

    TypeCondition typeCondition() const;
    void setTypeCondition(const TypeCondition &value);

    QList<TableFilterListModel::FilterInfo> filters() const;
    void setFilters(const QList<TableFilterListModel::FilterInfo> &filters);

    Q_INVOKABLE void append(int column, int role, const QVariant &value, bool negation = false, int id = 0);
    Q_INVOKABLE void set(int column, int role, const QVariant &value, bool negation = false, int id = 0);

    Q_INVOKABLE void appendColumnFilter(int column, const QVariant &value, bool negation = false, int id = 0);
    Q_INVOKABLE void appendRoleFilter(int role, const QVariant &value, bool negation = false, int id = 0);

    Q_INVOKABLE void removeAt(int i);
    Q_INVOKABLE void remove(int column, int role);

    Q_INVOKABLE void removeColumnFilter(int column);
    Q_INVOKABLE void removeRoleFilter(int role);

    Q_INVOKABLE void clear();

    bool isEmpty() const;
    int size() const;

    const TableFilterListModel::Filter &at(int i) const;

    QList<TableFilterListModel::FilterInfo>::const_iterator begin() const;
    QList<TableFilterListModel::FilterInfo>::const_iterator constBegin() const;

    QList<TableFilterListModel::FilterInfo>::const_iterator end() const;
    QList<TableFilterListModel::FilterInfo>::const_iterator constEnd() const;

signals:
    void typeConditionChanged();

protected:
    TypeCondition typeCondition_;
    QList <TableFilterListModel::FilterInfo> filters_;

    int indexOfFilters(const FilterIndex &index);

    void append(const FilterIndex &index, const Filter &filter);
};

#endif // TABLEFILTERLISTMODEL_H
