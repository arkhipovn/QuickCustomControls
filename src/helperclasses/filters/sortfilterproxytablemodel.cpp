#include "sortfilterproxytablemodel.h"
#include <QDebug>

SortFilterProxyTableModel::SortFilterProxyTableModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    filters_ = new TableFilterListModel(this);
    QObject::connect(filters_, &TableFilterListModel::typeConditionChanged, this, &SortFilterProxyTableModel::invalidateFilter);
    QObject::connect(filters_, &TableFilterListModel::rowsInserted, this, &SortFilterProxyTableModel::invalidateFilter);
    QObject::connect(filters_, &TableFilterListModel::rowsRemoved, this, &SortFilterProxyTableModel::invalidateFilter);
    QObject::connect(filters_, &TableFilterListModel::columnsInserted, this, &SortFilterProxyTableModel::invalidateFilter);
    QObject::connect(filters_, &TableFilterListModel::columnsRemoved, this, &SortFilterProxyTableModel::invalidateFilter);
    QObject::connect(filters_, &TableFilterListModel::modelReset, this, &SortFilterProxyTableModel::invalidateFilter);
    QObject::connect(filters_, &TableFilterListModel::dataChanged, this, &SortFilterProxyTableModel::invalidateFilter);
}

void SortFilterProxyTableModel::setSortColumn(int column)
{
    if(sortColumn() != column) {
        sort(column, QSortFilterProxyModel::sortOrder());
    }
}

void SortFilterProxyTableModel::setSortOrder(Qt::SortOrder order)
{
    if(sortOrder() != order) {
        sort(sortColumn(), order);
    }
}

QRegExp::PatternSyntax SortFilterProxyTableModel::patternSyntax() const
{
    return filterRegExp().patternSyntax();
}

void SortFilterProxyTableModel::setPatternSyntax(QRegExp::PatternSyntax patternSyntax)
{
    auto re = filterRegExp();
    re.setPatternSyntax(patternSyntax);
    setFilterRegExp(re);
    invalidateFilter();
}

QString SortFilterProxyTableModel::filterString() const
{
    return filterRegExp().pattern();
}

QString SortFilterProxyTableModel::filterRegularExpressionString() const
{
    return filterRegularExpression().pattern();
}

QModelIndexList SortFilterProxyTableModel::indexesFromSource(const QModelIndexList &indexes) const
{
    QModelIndexList list;
    for(auto r : indexes)
        list.append(mapToSource(r));
    return list;
}

#if QT_CONFIG(regularexpression)
QRegularExpression::PatternOptions SortFilterProxyTableModel::patternOptions() const
{
    return filterRegularExpression().patternOptions();
}

void SortFilterProxyTableModel::setPatternOptions(QRegularExpression::PatternOptions patternOptions)
{
    auto re = filterRegularExpression();
    re.setPatternOptions(patternOptions);
    setFilterRegularExpression(re);
    invalidateFilter();
}
#endif

bool SortFilterProxyTableModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // Проверка совпадения всех фильтров
    if(!filters_->isEmpty()) {
        QAbstractItemModel *model = sourceModel();
        switch (filters_->typeCondition()) {
            case TableFilterListModel::TypeCondition::And: {
                for(auto it = filters_->constBegin(); it != filters_->constEnd(); ++it) {
                    if(it->second.value.isNull())
                        continue;
                    auto modelIndex = model->index(sourceRow, it->first.column, sourceParent);
                    if(!modelIndex.isValid())
                        continue;
                    // получим значение в ячейке
                    auto value = model->data(modelIndex, it->first.role);
                    bool check = (value == it->second.value) == !it->second.negation;
                    if(!check) {
                        return false;    // Точно не подходит.. Больше даже проверять не будем..
                    }
                }
            }
                break;
            case TableFilterListModel::TypeCondition::Or: {
                bool filterAccepts = false;
                for(auto it = filters_->constBegin(); it != filters_->constEnd(); ++it) {
                    if(it->second.value.isNull())
                        continue;
                    auto modelIndex = model->index(sourceRow, it->first.column, sourceParent);
                    if(!modelIndex.isValid())
                        continue;
                    // получим значение в ячейке
                    auto value = model->data(modelIndex, it->first.role);
                    bool check = (value == it->second.value) == !it->second.negation;
                    if(check) {
                        filterAccepts = true;
                        break;      // Точно подходит. Выходим
                    }
                }

                if(!filterAccepts)
                    return false;    // ничего не нашли.. Выходим
            }
                break;
            default:
                break;
        }
    }

    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

TableFilterListModel *SortFilterProxyTableModel::filters() const
{
    return filters_;
}

int SortFilterProxyTableModel::roleKey(const QByteArray &role) const
{
    return roleNames().key(role, -1);
}



