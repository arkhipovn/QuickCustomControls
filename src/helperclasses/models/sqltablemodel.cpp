#include "sqltablemodel.h"
#include <QSqlDriver>
#include <QDebug>
#include <QSqlField>

SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase db) : QAbstractTableModel(parent),
    rows_(0),
    sortColumn_(-1),
    sortOrder_(Qt::SortOrder::AscendingOrder)
{
    db_ = db.isValid() ? db : QSqlDatabase::database();
}

bool SqlTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(role)
    if(orientation != Qt::Horizontal || section < 0 || columnCount() <= section)
        return false;
    if(horizontalHeader_.size() <= section)
        horizontalHeader_.resize(section + 1);
    horizontalHeader_[section] = value;
    emit headerDataChanged(orientation, section, section);
    return true;
}

QVariant SqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role)
    if (orientation == Qt::Horizontal) {
        QVariant val = horizontalHeader_.value(section);
        if(role == Qt::DisplayRole && !val.isValid())
            val = horizontalHeader_.value(section);
        if(val.isValid())
            return val;
        if(role == Qt::DisplayRole && records_.count() > section && column(section) != -1)
            return records_.fieldName(section);
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int SqlTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : rows_;
}

int SqlTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : records_.count();
}

QVariant SqlTableModel::data(const QModelIndex &index, int role) const
{
    QVariant v;
    if(!index.isValid())
        return v;
    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return v;
    if(!records_.isGenerated(index.column()))
        return v;
    auto newIndex = columnIndex(index);

    if(newIndex.row() >= rows_)
        return v;
    if(!query().seek(newIndex.row())) {
        return v;
    }

    return query().value(newIndex.column());
}

bool SqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;
    if(!index.isValid())
        return false;

    if(index.column() < 0 || index.column() >= columnCount())
        return false;

    if(index.row() < 0 || index.row() >= rowCount())
        return false;

    //    if (!(flags(index) & Qt::ItemIsEditable))
    //           return false;

    const QVariant prevValue = SqlTableModel::data(index, role);
    if(prevValue == value)
        return true;

    const QString upd = update(index, value);
    if(!upd.isEmpty()) {
        db_.exec(upd);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void SqlTableModel::clear()
{
    if(rows_ > 0) {
        beginRemoveRows(QModelIndex(), 0, rows_ - 1);
        rows_ = 0;
        endRemoveRows();
    }
    query_.clear();
}

void SqlTableModel::reset()
{
    beginResetModel();
    beginRemoveRows(QModelIndex(), 0, rows_ - 1);
    rows_ = 0;
    query_.clear();
    sortColumn_ = -1;
    sortOrder_ = Qt::SortOrder::AscendingOrder;

    filter_.clear();
    tableName_.clear();
    records_.clear();
    primaryIndex_.clear();

    columnOffsets_.clear();
    endRemoveRows();
    endResetModel();
}

void SqlTableModel::sort(int column, Qt::SortOrder order)
{
    sortColumn_ = column;
    sortOrder_ = order;
    QAbstractTableModel::sort(column, order);
    select();
}

void SqlTableModel::fetchMore(const QModelIndex &parent)
{
    // Чтобы узнать размер..
    Q_UNUSED(parent)
    if(query_.seek(0)) {
        int i = 1;  // тк запрос seek.. Он сразу же получает данные
        while(query_.next()) {
            ++i;
        }
        beginInsertRows(QModelIndex(), 0, i - 1);
        rows_ = i ;  // тк нумерация с 0
        endInsertRows();
    }
}

bool SqlTableModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return false;
}

bool SqlTableModel::select()
{
    const QString str = createSelect();
    if(str.isEmpty())
        return false;
    setQuery(QSqlQuery(str, db_));
    return !query_.isActive();
}

QString SqlTableModel::filter() const
{
    return filter_;
}

void SqlTableModel::setFilter(const QString &filter)
{
    if(filter_ != filter) {
        filter_ = filter;
        select();
    }
}

bool SqlTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if(count <= 0 || parent.isValid() || column < 0 || column > records_.count())
        return false;
    beginInsertColumns(parent, column, column + count - 1);
    for(int c = 0; c < count; ++c) {
        QSqlField field;
        field.setReadOnly(true);
        field.setGenerated(false);
        records_.insert(column, field);
        if(columnOffsets_.size() < records_.count()) {
            int offset = columnOffsets_.isEmpty() ? 0
                                                  : columnOffsets_[columnOffsets_.size() - 1];
            columnOffsets_.append(offset);
        }
        for(int i = column + 1; i < columnOffsets_.count(); ++i)
            ++columnOffsets_[i];
    }
    endInsertColumns();
    return true;
}

bool SqlTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    if(count <= 0 || parent.isValid() || column < 0 || column >= records_.count())
        return false;
    beginRemoveColumns(parent, column, column + count - 1);
    int i;
    for(i = 0; i < count; ++i)
        records_.remove(column);
    columnOffsets_.removeAt(i);
    for(i = column; i < columnOffsets_.count(); ++i) {
        columnOffsets_[i] = columnOffsets_[i] < count ? 0
                                                      : columnOffsets_[i] - count;
    }
    endRemoveColumns();
    return true;
}

void SqlTableModel::setQuery(const QSqlQuery &query)
{
    if(query_.lastQuery() != query.lastQuery()) {
        clear();
        query_ = query;
        fetchMore();
    }
}

QSqlQuery SqlTableModel::query() const
{
    return query_;
}

QString SqlTableModel::tableName() const
{
    return tableName_;
}

void SqlTableModel::setTableName(const QString &tableName)
{
    if(tableName_ != tableName) {
        clear();
        tableName_ = tableName;
        emit tableNameChanged();
        readRecords();
    }
}

QSqlDatabase SqlTableModel::db() const
{
    return db_;
}

void SqlTableModel::readRecords()
{
    columnOffsets_.clear();
    clear();
    records_.clear();

    if(tableName_.isEmpty())
        return;
    //    auto rec = db_.record(tableName_);        // почему-то не работает..
    // QSqlQuery q(createSelect(), db_);
    QSqlQuery q("SELECT * FROM '" + tableName_ + "' LIMIT 1", db_);
    records_ = q.record();
    primaryIndex_ =  db_.primaryIndex(tableName_);
    columnOffsets_ = QVector <int>(records_.count(), 0);
}

QString SqlTableModel::orderBy() const
{
    QString orderBy;
    if(sortColumn_ == -1)
        return orderBy;

    auto field = records_.field(sortColumn_);
    if(!field.isValid())
        return orderBy;

    if(!field.isGenerated())
        return orderBy;

    return " ORDER BY " + tableName_ + "." + field.name() + (sortOrder_ == Qt::AscendingOrder ? " ASC" : " DESC");
}

QString SqlTableModel::update(const QModelIndex &index, const QVariant &var) const
{
    Q_UNUSED(var)
    if(tableName_.isEmpty())
        return "";
    if(records_.isEmpty())
        return "";
    if(!records_.isGenerated(index.column()))
        return "";

    const QString name = tableName_.isEmpty() ? ""
                                              : "'" + tableName_ + "'";


    //    QSqlRecord priIndex = primaryIndex_.isEmpty() ? records_ : primaryIndex_;
    //    const QString selectStr = createSelect(priIndex);
    //    qDebug() << selectStr;
    //    if(selectStr.isEmpty())
    //        return "";
    //    QSqlQuery query(selectStr, db_);
    //    if(query.seek(0)) {
    //        if(query_.next()) {
    //            qDebug() << query_.value(0).toString();
    //        }
    //    }

    return "";
    //    return "UPDATE " + name + " SET " + records_.fieldName(index.column()) + " = '" + var.toString() + "'";
}

QString SqlTableModel::where() const
{
    return filter_.isEmpty() ? ""
                             : " WHERE " + filter_;
}

QString SqlTableModel::createSelect() const
{
    return createSelect(records_);
}

QString SqlTableModel::createSelect(const QSqlRecord &records) const
{
    if(tableName_.isEmpty())
        return "";
    if(records.isEmpty())
        return "";

    //@N передалать бы..
    const QString name = tableName_.isEmpty() ? ""
                                              : "'" + tableName_ + "'";

    QString s;
    for(int i = 0; i < records.count(); ++i) {
        if(!records.isGenerated(i))
            continue;
        if(!s.isEmpty()) s.append(", ");
        s.append(records.fieldName(i));
    }

    if(s.isEmpty())
        return s;

    return "SELECT " + s + " FROM " + name + where() + orderBy();
}

int SqlTableModel::column(int modelColumn) const
{
    if (modelColumn < 0 || modelColumn >= records_.count() || !records_.isGenerated(modelColumn) || modelColumn >= columnOffsets_.size())
        return -1;
    return modelColumn - columnOffsets_[modelColumn];
}

QModelIndex SqlTableModel::columnIndex(const QModelIndex &index) const
{
    int modelColumn = column(index.column());
    return modelColumn < 0 ? QModelIndex()
                           : createIndex(index.row(), modelColumn, index.internalPointer());
}



