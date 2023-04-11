#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QItemSelectionModel>
#include <QJSValue>

class SelectionModel : public QItemSelectionModel
{
    Q_OBJECT

    Q_PROPERTY(SelectionBehavior selectionBehavior READ getSelectionBehavior WRITE setSelectionBehavior NOTIFY selectionBehaviorChanged)
    Q_PROPERTY(SelectionMode selectionMode READ getSelectionMode WRITE setSelectionMode NOTIFY selectionModeChanged)
    Q_PROPERTY(QJSValue roleProvider READ getRoleProvider WRITE setRoleProvider NOTIFY roleProviderChanged)

public:
    explicit SelectionModel(QAbstractItemModel *model = nullptr, QObject *parent = nullptr);

    enum SelectionBehavior {
        SelectItems = 0,
        SelectRows,
        SelectColumns
    };
    Q_ENUMS(SelectionBehavior)

    enum SelectionMode {
        NoSelection = 0,
        SingleSelection,
        MultiSelection
    };
    Q_ENUMS(SelectionMode)

    SelectionBehavior getSelectionBehavior() const;
    void setSelectionBehavior(const SelectionBehavior &value);

    SelectionMode getSelectionMode() const;
    void setSelectionMode(const SelectionMode &value);

    QJSValue getRoleProvider() const;
    void setRoleProvider(const QJSValue &value);

public slots:
    void select(const QModelIndex &index, bool clear = true);

    void selection(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void selectRow(int row, bool clear = true);
    void selectColumn(int column, bool clear = true);
    void selectAll();

    void setCurrentIndex(const QModelIndex &index, bool clear = true);

    void increaseRowIndex(bool clear = true);
    void decreaseRowIndex(bool clear = true);

    void increaseColumnIndex(bool clear = true);
    void decreaseColumnIndex(bool clear = true);

    void copy(const QStringList &verticalHeader = {}, const QStringList &horizontalHeader = {});    // сохраняет в буфер


#ifdef USING_WIDGETS
    void saveToCSV(const QStringList &verticalHeader = {}, const QStringList &horizontalHeader = {}, bool fullTable = false);
    void printTable(const QStringList &verticalHeader = {}, const QStringList &horizontalHeader = {}, bool fullTable = false);
#endif
    void saveToCSV(const QString &file, const QStringList &verticalHeader = {}, const QStringList &horizontalHeader = {}, bool fullTable = false);

signals:
    void selectionBehaviorChanged();
    void selectionModeChanged();
    void roleProviderChanged();

private:
    SelectionBehavior selectionBehavior;
    SelectionMode selectionMode;

    QJSValue roleProvider;

    // fullTable - это значит не нужно смотреть на то, что выделено. Нужно печатать всю таблицу..
    QStringList getSelectedText(const QString &sep, const QStringList &verticalHeader = {}, const QStringList &horizontalHeader = {}, bool fullTable = false);


    // потом разделить..
    //    QStringList getSelectedItems(const QString &sep);
    //    QStringList getSelectedRows(const QString &sep);
    //    QStringList getSelectedColumns(const QString &sep);

};

#endif // SELECTIONMODEL_H
