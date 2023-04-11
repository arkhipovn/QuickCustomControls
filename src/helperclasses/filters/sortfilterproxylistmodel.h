#ifndef SORTFILTERPROXYLISTMODEL_H
#define SORTFILTERPROXYLISTMODEL_H

#include <QtCore/qsortfilterproxymodel.h>

#ifdef QT_QML_LIB // Q_QML_EXPORT
#include <QtQml/qqmlparserstatus.h> //@N А это нужно? проверить
#endif
#include <QDebug>
class SortFilterProxyListModel : public QSortFilterProxyModel
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
    Q_PROPERTY(QByteArray sortRole READ sortRole WRITE setSortRole)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder)

    // Filter
    Q_PROPERTY(QByteArray filterRole READ filterRole WRITE setFilterRole)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString)
    Q_PROPERTY(FilterSyntax filterSyntax READ filterSyntax WRITE setFilterSyntax)

public:
    explicit SortFilterProxyListModel(QObject *parent = nullptr);

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
    QByteArray sortRole() const;
    void setSortRole(const QByteArray &role);

    void setSortOrder(Qt::SortOrder order);

    // Search Filter
    FilterSyntax filterSyntax() const;
    void setFilterSyntax(FilterSyntax syntax);

    // Это в случае поиска по все таблице можно задать роль в которой искать
    QByteArray filterRole() const;
    void setFilterRole(const QByteArray &role);

    QString filterString() const;
    void setFilterString(const QString &filter);

    enum TypeCondition {
        AND,
        OR
    };
    Q_ENUMS(TypeCondition)


    struct Filter {
        QString text;
        bool compare = true;    // true - значит все, что совпадают, false - не совпадают
    };


    //    QModelIndex mapFromSource(const QModelIndex &src) const override {
    //        qDebug() << "mapFromSource";
    //        if (!src.isValid() || !sourceModel()) return {};
    //        Q_ASSERT(src.model() == sourceModel());
    //        return createIndex(src.row(), src.column(), src.internalPointer());
    //    }
    //    QModelIndex mapToSource(const QModelIndex &prx) const override {
    //        qDebug() << "mapToSource";
    //        if (!prx.isValid() || !sourceModel()) return {};
    //        Q_ASSERT(prx.model() == this);
    //        return sourceModel()->index(prx.row(), prx.column()/*, prx.internalPointer()*/);
    //    }
    //    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override {
    //        qDebug() << "index";
    //        if (!sourceModel()) return {};
    //        Q_ASSERT(!parent.isValid() || parent.model() == this);
    //        return mapFromSource(sourceModel()->index(row, column, mapToSource(parent)));
    //    }



public slots:
    void appendFilter(const QByteArray &role, const QString &filter, bool compare = true, TypeCondition type = TypeCondition::AND);
    void removeFilter(const QByteArray &role, TypeCondition type = TypeCondition::AND);

    void clearFilters();

    QModelIndexList indexFromSource(const QModelIndexList &indexes) const;

protected:
    QHash<int, QByteArray> roleNames() const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

#ifdef QT_QML_LIB // Q_QML_EXPORT
    void classBegin();
    void componentComplete();
#endif

    int roleKey(const QByteArray &role) const;

signals:
    void countChanged();

private:
    // список фильтров по key role
    QMap <int, Filter> orFilters;
    QMap <int, Filter> andFilters;

    QByteArray filterRole_;
    QByteArray sortRole_;

#ifdef QT_QML_LIB // Q_QML_EXPORT
    bool complete;
#endif
};

#endif // SORTFILTERPROXYLISTMODEL_H
