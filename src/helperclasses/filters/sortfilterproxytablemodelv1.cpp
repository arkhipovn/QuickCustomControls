#include "sortfilterproxytablemodelv1.h"

#include <QDebug>

SortFilterProxyTableModelV1::SortFilterProxyTableModelV1(QObject *parent) : QSortFilterProxyModel(parent)
  #ifdef QT_QML_LIB // Q_QML_EXPORT
  , complete(false)
  #endif
{
    filterColumn = -1;
    sortColumn = -1;
    QObject::connect(this, &QSortFilterProxyModel::rowsInserted, this, &SortFilterProxyTableModelV1::countChanged);
    QObject::connect(this, &QSortFilterProxyModel::rowsRemoved, this, &SortFilterProxyTableModelV1::countChanged);
}

#ifdef QT_QML_LIB // Q_QML_EXPORT
void SortFilterProxyTableModelV1::classBegin()
{

}

void SortFilterProxyTableModelV1::componentComplete()
{
    complete = true;
}
#endif

QObject *SortFilterProxyTableModelV1::source() const
{
    return sourceModel();
}

void SortFilterProxyTableModelV1::setSource(QObject *source)
{
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));
    //    QObject::connect(this, &SortFilterProxyTableModelV1::layoutChanged, this, [&](){
    //        qDebug() << "this layoutChanged";
    //    });

    //    QObject::connect(qobject_cast<QAbstractItemModel *>(source), &QAbstractItemModel::layoutChanged, this, [&](){
    //        qDebug() << "sourceModel layoutChanged";
    //    });
}

int SortFilterProxyTableModelV1::count() const
{
    return rowCount();
}

void SortFilterProxyTableModelV1::sort(int column, Qt::SortOrder order)
{
    sortColumn = column;
    setSortOrder(order);
}

int SortFilterProxyTableModelV1::getSortColumn() const
{
    return sortColumn;
}

void SortFilterProxyTableModelV1::setSortColumn(int value)
{
    if(sortColumn != value) {
        sortColumn = value;
        if(sortColumn == -1) return;
        QSortFilterProxyModel::sort(sortColumn, QSortFilterProxyModel::sortOrder());
    }
}

void SortFilterProxyTableModelV1::setSortOrder(Qt::SortOrder order)
{
    if(sortColumn == -1) return;
    QSortFilterProxyModel::sort(sortColumn, order);
}

SortFilterProxyTableModelV1::FilterSyntax SortFilterProxyTableModelV1::filterSyntax() const
{
    return static_cast<FilterSyntax>(filterRegExp().patternSyntax());
}

void SortFilterProxyTableModelV1::setFilterSyntax(SortFilterProxyTableModelV1::FilterSyntax syntax)
{
    setFilterRegExp(QRegExp(filterString(), filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(syntax)));
}

QString SortFilterProxyTableModelV1::filterString() const
{
    return filterRegExp().pattern();
}

void SortFilterProxyTableModelV1::setFilterString(const QString &filter)
{
    setFilterRegExp(QRegExp(filter, filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(filterSyntax())));
}

void SortFilterProxyTableModelV1::appendFilter(int column, const QString &filter, bool compare, SortFilterProxyTableModelV1::TypeCondition type)
{
    if(column == -1) return;

    switch (type) {
        case TypeCondition::AND: {
            auto it = andFilters.find(column);
            if(it == andFilters.end()) {
                if(filter.isEmpty()) return;
                else {
                    Filter f; f.text = filter; f.compare = compare;
                    andFilters.insert(column, f);
                }
            }
            else {
                if(filter.isEmpty()) andFilters.erase(it);
                else {
                    it.value().text = filter;
                    it.value().compare = compare;
                }
            }
            break;
        }
        case TypeCondition::OR: {
            auto it = orFilters.find(column);
            if(it == orFilters.end()) {
                if(filter.isEmpty()) return;
                else {
                    Filter f; f.text = filter; f.compare = compare;
                    orFilters.insert(column, f);
                }
            }
            else {
                if(filter.isEmpty()) orFilters.erase(it);
                else {
                    it.value().text = filter;
                    it.value().compare = compare;
                }
            }
        }
        default: return;
    }

    invalidateFilter();
}

void SortFilterProxyTableModelV1::clearFilters()
{
    orFilters.clear();
    andFilters.clear();
}

QModelIndexList SortFilterProxyTableModelV1::indexFromSource(const QModelIndexList &indexes) const
{
    QModelIndexList list;
    for(auto r : indexes)
        list.append(mapToSource(r));
    return list;
}

QHash<int, QByteArray> SortFilterProxyTableModelV1::roleNames() const
{
    if(QAbstractItemModel *source = sourceModel())
        return source->roleNames();
    return QHash<int, QByteArray>();
}

bool SortFilterProxyTableModelV1::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    //    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);

    QAbstractItemModel *model = sourceModel();
    QRegExp rx = filterRegExp();

    // Проверка совпадения всех фильтров AND
    if(!andFilters.isEmpty()) {
        auto it = andFilters.constBegin() - 1;
        while(++it != andFilters.constEnd()) {
            if(it.key() >= model->columnCount()) continue;
            auto sourceIndex = model->index(sourceRow, it.key(), sourceParent);
            if(!sourceIndex.isValid()) continue;
            // получим значение в ячейке
            QString key = model->data(sourceIndex).toString();
            if(!key.contains(QRegExp(it.value().text)) == it.value().compare) return false;    // Точно не подходит.. Больше даже проверять не будем..
        }
    }

    // Проверка совпадения всех фильтров OR
    if(!orFilters.isEmpty()) {
        bool checkFilter = false;
        auto it = orFilters.constBegin() - 1;
        while(++it != orFilters.constEnd()) {
            if(it.key() >= model->columnCount()) continue;
            auto sourceIndex = model->index(sourceRow, it.key(), sourceParent);
            if(!sourceIndex.isValid()) continue;

            // получим значение в ячейке
            QString key = model->data(sourceIndex).toString();
            if(key.contains(QRegExp(it.value().text)) == it.value().compare) {
                checkFilter = true;
                break; // Точно подходит. Выходим
            }
        }
        if(!checkFilter) return false;    // ничего не нашли.. Выходим
    }

    if(!rx.isEmpty()) {
        if(filterColumn == -1) {
            // нет роли для поиска.. Смотрим всю таблицу
            bool checkFilter = false;
            for(int i = 0; i < model->columnCount(); ++i) {
                auto sourceIndex = model->index(sourceRow, i, sourceParent);
                QString key = model->data(sourceIndex).toString();
                if(key.contains(rx)) {
                    checkFilter = true;
                    break; // Точно подходит. Выходим
                }
            }

            if(!checkFilter) return false;    // ничего не нашли.. Выходим
        }
        else {
            if(filterColumn < model->columnCount()) {
                auto sourceIndex = model->index(sourceRow, filterColumn, sourceParent);
                if(sourceIndex.isValid()) {
                    QString key = model->data(sourceIndex).toString();
                    if(!key.contains(rx)) return false;
                }
            }
        }
    }

    return true;
}

int SortFilterProxyTableModelV1::getFilterColumn() const
{
    return filterColumn;
}

void SortFilterProxyTableModelV1::setFilterColumn(int value)
{
    if(filterColumn != value) {
        filterColumn = value;
        invalidateFilter();
    }
}

