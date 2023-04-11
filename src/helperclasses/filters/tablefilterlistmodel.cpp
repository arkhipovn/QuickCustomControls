#include "tablefilterlistmodel.h"

bool TableFilterListModel::FilterIndex::operator ==(const TableFilterListModel::FilterIndex &index) const
{
    return column == index.column && role == index.role;
}

TableFilterListModel::TableFilterListModel(QObject *parent) : QAbstractListModel(parent),
    typeCondition_(TypeCondition::And)
{

}

QHash<int, QByteArray> TableFilterListModel::roleNames() const
{
    static const QHash <int, QByteArray> roles = QHash <int, QByteArray> {
        { Role::FilterColumn, "filterColumn" },
        { Role::FilterRole, "filterRole" },
        { Role::FilterValue, "filterValue" },
        { Role::FilterId, "filterId" },
        { Role::FilterNegation, "filterNegation" }
    };
    return roles;
}

QVariant TableFilterListModel::data(const QModelIndex &index, int role) const
{
    QVariant v;
    if(!index.isValid())
        return v;
    if(index.row() < 0 || index.row() >= filters_.size())
        return v;

    switch (role) {
        case Role::FilterColumn:
            return filters_.at(index.row()).first.column;
        case Role::FilterRole:
            return filters_.at(index.row()).first.role;
        case Role::FilterValue:
            return filters_.at(index.row()).second.value;
        case Role::FilterId:
            return filters_.at(index.row()).second.id;
        case Role::FilterNegation:
            return filters_.at(index.row()).second.negation;
        default:
            break;
    }

    return v;
}

bool TableFilterListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!value.isValid())
        return false;
    if(index.row() < 0 || index.row() >= filters_.size())
        return false;

    switch (role) {
        case Role::FilterValue:
            if(filters_.at(index.row()).second.value != value) {
                filters_[index.row()].second.value = value;
                emit dataChanged(index, index, { role });
            }
            break;
        case Role::FilterNegation:
            if(filters_.at(index.row()).second.negation != value.toBool()) {
                filters_[index.row()].second.negation = value.toBool();
                emit dataChanged(index, index, { role });
            }
            break;
        default:
            break;
    }

    return true;
}

int TableFilterListModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return filters_.size();
}

TableFilterListModel::TypeCondition TableFilterListModel::typeCondition() const
{
    return typeCondition_;
}

void TableFilterListModel::setTypeCondition(const TypeCondition &value)
{
    if(typeCondition_ != value) {
        typeCondition_ = value;
        emit typeConditionChanged();
    }
}

QList<TableFilterListModel::FilterInfo> TableFilterListModel::filters() const
{
    return filters_;
}

void TableFilterListModel::setFilters(const QList<TableFilterListModel::FilterInfo> &filters)
{
    clear();
    if(!filters.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, filters.size() - 1);
        filters_ = filters;
        endInsertRows();
    }
}

void TableFilterListModel::append(int column, int role, const QVariant &value, bool negation, int id)
{
    if(!value.isValid())
        return;

    FilterIndex filterIndex;
    filterIndex.column = column;
    filterIndex.role = role;

    int index = indexOfFilters(filterIndex);

    if(index != -1) {
        if(value == filters_.at(index).second.value)
            return;
    }

    Filter filter;
    filter.id = id;
    filter.value = value;
    filter.negation = negation;

    append(filterIndex, filter);
}
#include <QDebug>
void TableFilterListModel::set(int column, int role, const QVariant &value, bool negation, int id)
{
    if(!value.isValid())
        return;

    FilterIndex filterIndex;
    filterIndex.column = column;
    filterIndex.role = role;

    Filter filter;
    filter.id = id;
    filter.value = value;
    filter.negation = negation;

    int index = indexOfFilters(filterIndex);

    if(index != -1) {
        filters_[index].second = filter;
        emit dataChanged(this->index(index), this->index(index), { Role::FilterValue, Role::FilterId, Role::FilterNegation });
    }
    else append(filterIndex, filter);
}

void TableFilterListModel::appendColumnFilter(int column, const QVariant &value, bool negation, int id)
{
    append(column, Qt::DisplayRole, value, negation, id);
}

void TableFilterListModel::appendRoleFilter(int role, const QVariant &value, bool negation, int id)
{
    append(0, role, value, negation, id);
}

void TableFilterListModel::removeAt(int i)
{
    Q_ASSERT_X(i < 0 || i >= size(), "TableFilterListModel::remove", "index out of range");
    beginRemoveRows(QModelIndex(), i, i);
    filters_.removeAt(i);
    endRemoveRows();
}

void TableFilterListModel::remove(int column, int role)
{
    FilterIndex filterIndex;
    filterIndex.column = column;
    filterIndex.role = role;
    int i = indexOfFilters(filterIndex);
    if(i != -1) {
        removeAt(i);
    }
}

void TableFilterListModel::removeColumnFilter(int column)
{
    remove(column, Qt::DisplayRole);
}

void TableFilterListModel::removeRoleFilter(int role)
{
    remove(0, role);
}

void TableFilterListModel::clear()
{
    if(!filters_.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, filters_.size() - 1);
        filters_.clear();
        endRemoveRows();
    }
}

bool TableFilterListModel::isEmpty() const
{
    return filters_.isEmpty();
}

int TableFilterListModel::size() const
{
    return filters_.size();
}

const TableFilterListModel::Filter &TableFilterListModel::at(int i) const
{
    Q_ASSERT_X(i < 0 || i >= size(), "TableFilterListModel::at", "index out of range");
    return filters_.at(i).second;
}

QList<TableFilterListModel::FilterInfo>::const_iterator TableFilterListModel::begin() const
{
    return filters_.begin();
}

QList<TableFilterListModel::FilterInfo>::const_iterator TableFilterListModel::constBegin() const
{
    return filters_.constBegin();
}

QList<TableFilterListModel::FilterInfo>::const_iterator TableFilterListModel::end() const
{
    return filters_.end();
}

QList<TableFilterListModel::FilterInfo>::const_iterator TableFilterListModel::constEnd() const
{
    return filters_.constEnd();
}

int TableFilterListModel::indexOfFilters(const TableFilterListModel::FilterIndex &index)
{
    for(int i = 0; i < filters_.size(); ++i) {
        if(filters_.at(i).first == index) {
            return i;
        }
    }
    return -1;
}

void TableFilterListModel::append(const FilterIndex &index, const Filter &filter)
{
    beginInsertRows(QModelIndex(), filters_.size(), filters_.size());
    filters_.append(qMakePair(index, filter));
    endInsertRows();
}
