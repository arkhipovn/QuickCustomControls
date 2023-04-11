#include "sortfilterproxylistmodel.h"
#include <QDebug>

SortFilterProxyListModel::SortFilterProxyListModel(QObject *parent) : QSortFilterProxyModel(parent)
  #ifdef QT_QML_LIB // Q_QML_EXPORT
  , complete(false)
  #endif
{
    QObject::connect(this, &QSortFilterProxyModel::rowsInserted, this, &SortFilterProxyListModel::countChanged);
    QObject::connect(this, &QSortFilterProxyModel::rowsRemoved, this, &SortFilterProxyListModel::countChanged);
}

#ifdef QT_QML_LIB // Q_QML_EXPORT
void SortFilterProxyListModel::classBegin()
{

}

void SortFilterProxyListModel::componentComplete()
{
    complete = true;
    if (!sortRole_.isEmpty())
        QSortFilterProxyModel::setSortRole(roleKey(sortRole_));
    if(!filterRole_.isEmpty())
        QSortFilterProxyModel::setFilterRole(roleKey(filterRole_));
}
#endif

QObject *SortFilterProxyListModel::source() const
{
    return sourceModel();
}

void SortFilterProxyListModel::setSource(QObject *source)
{
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));

    /*

    if(sourceModel()) disconnect(sourceModel(), 0, this, 0);
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));

    if(!sourceModel()) return;
    QObject::connect(sourceModel(), &QAbstractItemModel::dataChanged, this, &SortFilterProxyListModel::dataChanged);
    QObject::connect(sourceModel(), &QAbstractItemModel::headerDataChanged, this, &SortFilterProxyListModel::headerDataChanged);

    QObject::connect(sourceModel(), &QAbstractItemModel::layoutAboutToBeChanged, this, &SortFilterProxyListModel::layoutAboutToBeChanged);
    QObject::connect(sourceModel(), &QAbstractItemModel::layoutChanged, this, &SortFilterProxyListModel::layoutChanged);

    QObject::connect(sourceModel(), &QAbstractItemModel::rowsAboutToBeInserted, this, &SortFilterProxyListModel::rowsAboutToBeInserted);
    QObject::connect(sourceModel(), &QAbstractItemModel::rowsInserted, this, &SortFilterProxyListModel::rowsInserted);

    QObject::connect(sourceModel(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &SortFilterProxyListModel::rowsAboutToBeRemoved);
    QObject::connect(sourceModel(), &QAbstractItemModel::rowsRemoved, this, &SortFilterProxyListModel::rowsRemoved);

    QObject::connect(sourceModel(), &QAbstractItemModel::rowsAboutToBeMoved, this, &SortFilterProxyListModel::rowsAboutToBeMoved);
    QObject::connect(sourceModel(), &QAbstractItemModel::rowsMoved, this, &SortFilterProxyListModel::rowsMoved);

    QObject::connect(sourceModel(), &QAbstractItemModel::columnsAboutToBeInserted, this, &SortFilterProxyListModel::columnsAboutToBeInserted);
    QObject::connect(sourceModel(), &QAbstractItemModel::columnsInserted, this, &SortFilterProxyListModel::columnsInserted);

    QObject::connect(sourceModel(), &QAbstractItemModel::columnsAboutToBeRemoved, this, &SortFilterProxyListModel::columnsAboutToBeRemoved);
    QObject::connect(sourceModel(), &QAbstractItemModel::columnsRemoved, this, &SortFilterProxyListModel::columnsRemoved);

    QObject::connect(sourceModel(), &QAbstractItemModel::columnsAboutToBeMoved, this, &SortFilterProxyListModel::columnsAboutToBeMoved);
    QObject::connect(sourceModel(), &QAbstractItemModel::columnsMoved, this, &SortFilterProxyListModel::columnsMoved);

    QObject::connect(sourceModel(), &QAbstractItemModel::modelAboutToBeReset, this, &SortFilterProxyListModel::modelAboutToBeReset);
    QObject::connect(sourceModel(), &QAbstractItemModel::modelReset, this, &SortFilterProxyListModel::modelReset);
*/
}


int SortFilterProxyListModel::count() const
{
    return rowCount();
}

QByteArray SortFilterProxyListModel::sortRole() const
{
    return sortRole_;
}

void SortFilterProxyListModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);
}

void SortFilterProxyListModel::setSortRole(const QByteArray &role)
{
    if(sortRole_ != role) {
        sortRole_ = role;
        if(complete) QSortFilterProxyModel::setSortRole(roleKey(role));
    }
}

void SortFilterProxyListModel::setSortOrder(Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(0, order);
}

SortFilterProxyListModel::FilterSyntax SortFilterProxyListModel::filterSyntax() const
{
    return static_cast<FilterSyntax>(filterRegExp().patternSyntax());
}

void SortFilterProxyListModel::setFilterSyntax(SortFilterProxyListModel::FilterSyntax syntax)
{
    setFilterRegExp(QRegExp(filterString(), filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(syntax)));
}

QByteArray SortFilterProxyListModel::filterRole() const
{
    return filterRole_;
}

void SortFilterProxyListModel::setFilterRole(const QByteArray &role)
{
    if(filterRole_ != role) {
        filterRole_ = role;
        if(complete) QSortFilterProxyModel::setFilterRole(roleKey(role));
    }
}

QString SortFilterProxyListModel::filterString() const
{
    return filterRegExp().pattern();
}

void SortFilterProxyListModel::setFilterString(const QString &filter)
{
    setFilterRegExp(QRegExp(filter, filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(filterSyntax())));
}

void SortFilterProxyListModel::appendFilter(const QByteArray &role, const QString &filter, bool compare, SortFilterProxyListModel::TypeCondition type)
{
    if(role.isEmpty()) return;

    auto key = roleKey(role);
    if(key == -1) return;

    switch (type) {
        case TypeCondition::AND: {
            auto it = andFilters.find(key);
            if(it == andFilters.end()) {
                if(filter.isEmpty()) return;
                else {
                    Filter f; f.text = filter; f.compare = compare;
                    andFilters.insert(key, f);
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
            auto it = orFilters.find(key);
            if(it == orFilters.end()) {
                if(filter.isEmpty()) return;
                else {
                    Filter f; f.text = filter; f.compare = compare;
                    orFilters.insert(key, f);
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

void SortFilterProxyListModel::removeFilter(const QByteArray &role, TypeCondition type)
{
    auto key = roleKey(role);
    if(key == -1) return;

    switch (type) {
        case TypeCondition::AND: {
            auto it = andFilters.find(key);
            if(it == andFilters.end()) return;
            else andFilters.erase(it);
            break;
        }
        case TypeCondition::OR: {
            auto it = orFilters.find(key);
            if(it == orFilters.end()) return;
            else orFilters.erase(it);
            break;
        }
        default: return;
    }

    invalidateFilter();
}

void SortFilterProxyListModel::clearFilters()
{
    orFilters.clear();
    andFilters.clear();
}

QModelIndexList SortFilterProxyListModel::indexFromSource(const QModelIndexList &indexes) const
{
    QModelIndexList list;
    for(auto r : indexes)
        list.append(mapToSource(r));
    return list;
}

QHash<int, QByteArray> SortFilterProxyListModel::roleNames() const
{
    if(QAbstractItemModel *source = sourceModel())
        return source->roleNames();
    return QHash<int, QByteArray>();
}

bool SortFilterProxyListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QAbstractItemModel *model = sourceModel();

    auto sourceIndex = model->index(sourceRow, 0, sourceParent);
    if(!sourceIndex.isValid()) return true;

    // Проверка совпадения всех фильтров AND
    if(!andFilters.isEmpty()) {
        auto it = andFilters.constBegin();
        while(it != andFilters.constEnd()) {
            // получим значение в ячейке
            QString key = model->data(sourceIndex, it.key()).toString();
            if((key.compare(it.value().text) == 0) != it.value().compare) return false;// Точно не подходит.. Больше даже проверять не будем..
            ++it;
        }
        // прошли все проверки..
    }

    // Проверка совпадения всех фильтров OR
    if(!orFilters.isEmpty()) {
        bool checkFilter = false;
        auto it = orFilters.constBegin();
        while(it != orFilters.constEnd()) {
            QString key = model->data(sourceIndex, it.key()).toString();
            if((key.compare(it.value().text) == 0) == it.value().compare) {
                //            if(key.contains(QRegExp(it.value().text)) == it.value().compare) {
                checkFilter = true;
                break; // Точно подходит. Выходим
            }
            ++it;
        }
        if(!checkFilter) return false;    // ничего не нашли.. Выходим
    }

    QRegExp rx = filterRegExp();
    // смотрим есть ли общий фильтр поиска
    if(!rx.isEmpty()) {
        // нет роли для поиска.. Смотрим всю таблицу
        if(filterRole().isEmpty()) {
            bool checkFilter = false;
            auto roles = roleNames();
            auto it = roles.constBegin();
            while(it != roles.constEnd()) {
                QString key = model->data(sourceIndex, it.key()).toString();
                if(key.contains(rx)) {
                    checkFilter = true;
                    break; // Точно подходит. Выходим
                }
                ++it;
            }
            if(!checkFilter) return false;    // ничего не нашли.. Выходим
        }
        else {
            QString key = model->data(sourceIndex, roleKey(filterRole())).toString();
            if(!key.contains(rx)) return false;
        }
    }
    return true;
}

int SortFilterProxyListModel::roleKey(const QByteArray &role) const
{
    QHash<int, QByteArray> roles = roleNames();
    QHashIterator<int, QByteArray> it(roles);
    while (it.hasNext()) {
        it.next();
        if (it.value() == role)
            return it.key();
    }
    return -1;
}


