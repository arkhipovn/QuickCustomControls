#ifndef SORTFILTERPROXYTABLEMODELV1_H
#define SORTFILTERPROXYTABLEMODELV1_H

#include <QtCore/qsortfilterproxymodel.h>

#ifdef QT_QML_LIB // Q_QML_EXPORT
#include <QtQml/qqmlparserstatus.h> //@N А это нужно? проверить
#endif
#include <QDebug>
class SortFilterProxyTableModelV1 : public QSortFilterProxyModel
        #ifdef QT_QML_LIB // Q_QML_EXPORT
        , public QQmlParserStatus
        #endif
{
    Q_OBJECT
#ifdef QT_QML_LIB // Q_QML_EXPORT
    Q_INTERFACES(QQmlParserStatus)
#endif

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QObject *source READ source WRITE setSource)

    // Sort
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder)
    Q_PROPERTY(int sortColumn READ getSortColumn WRITE setSortColumn)

    // Filter
    Q_PROPERTY(int filterColumn READ getFilterColumn WRITE setFilterColumn)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString)
    Q_PROPERTY(FilterSyntax filterSyntax READ filterSyntax WRITE setFilterSyntax)

public:
    explicit SortFilterProxyTableModelV1(QObject *parent = nullptr);

    QObject *source() const;
    void setSource(QObject *source);

    int count() const;

    enum FilterSyntax {
        RegExp,
        Wildcard,
        FixedString
    };
    Q_ENUMS(FilterSyntax)

    Q_INVOKABLE void sort(int column, Qt::SortOrder order) override;

    // Sort
    void setSortOrder(Qt::SortOrder order);

    int getSortColumn() const;
    void setSortColumn(int value);


    // Search Filter
    FilterSyntax filterSyntax() const;
    void setFilterSyntax(FilterSyntax syntax);

    QString filterString() const;
    void setFilterString(const QString &filter);

    enum TypeCondition {
        AND,
        OR
    };
    Q_ENUMS(TypeCondition)

    int getFilterColumn() const;
    void setFilterColumn(int value);

    struct Filter {
        QString text;
        bool compare = true;    // true - значит все, что совпадают, false - не совпадают
    };

    Q_INVOKABLE void appendFilter(int column, const QString &filter, bool compare = true, TypeCondition type = TypeCondition::AND);
    Q_INVOKABLE void clearFilters();
    Q_INVOKABLE QModelIndexList indexFromSource(const QModelIndexList &indexes) const;

protected:
    QHash<int, QByteArray> roleNames() const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

#ifdef QT_QML_LIB // Q_QML_EXPORT
    void classBegin();
    void componentComplete();
#endif

signals:
    void countChanged();

private:
    // список фильтров по key column
    QMap <int, Filter> orFilters;
    QMap <int, Filter> andFilters;

    int filterColumn;
    int sortColumn;

#ifdef QT_QML_LIB // Q_QML_EXPORT
    bool complete;
#endif
};

#endif // SORTFILTERPROXYTABLEMODELV1_H
