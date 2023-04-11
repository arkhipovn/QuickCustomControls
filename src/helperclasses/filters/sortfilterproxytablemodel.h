#ifndef SORTFILTERPROXYTABLEMODEL_H
#define SORTFILTERPROXYTABLEMODEL_H

#include <QSortFilterProxyModel>
#include "tablefilterlistmodel.h"
#include <QDebug>

class SortFilterProxyTableModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder)
    Q_PROPERTY(int sortColumn READ sortColumn WRITE setSortColumn)

    Q_PROPERTY(QString filterWildcardString READ filterString WRITE setFilterWildcard)
    Q_PROPERTY(QString filterFixedString READ filterString WRITE setFilterFixedString)

#if QT_CONFIG(regularexpression)
    Q_PROPERTY(QString filterRegularString READ filterRegularExpressionString WRITE setFilterRegularExpression)
#endif

    Q_PROPERTY(TableFilterListModel *filters READ filters CONSTANT)

public:
    explicit SortFilterProxyTableModel(QObject *parent = nullptr);

    // Sort
    void setSortColumn(int column);
    void setSortOrder(Qt::SortOrder order);

    // Filter
    QRegExp::PatternSyntax patternSyntax() const;
    void setPatternSyntax(QRegExp::PatternSyntax patternSyntax);

#if QT_CONFIG(regularexpression)
    QRegularExpression::PatternOptions patternOptions() const;
    void setPatternOptions(QRegularExpression::PatternOptions patternOptions);
#endif

    QString filterString() const;
    QString filterRegularExpressionString() const;

    Q_INVOKABLE QModelIndexList indexesFromSource(const QModelIndexList &indexes) const;

    TableFilterListModel *filters() const;

    Q_INVOKABLE int roleKey(const QByteArray &role) const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    TableFilterListModel *filters_;
};

#endif // SORTFILTERPROXYTABLEMODEL_H
