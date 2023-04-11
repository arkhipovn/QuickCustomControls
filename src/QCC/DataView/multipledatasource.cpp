#include "multipledatasource.h"

MultipleDataSource::MultipleDataSource(QObject *parent) : QAbstractTableModel(parent)
{

}

QHash<int, QByteArray> MultipleDataSource::roleNames() const
{
    return { {Qt::DisplayRole, "display"}, {Qt::BackgroundRole, "background"} };
}

