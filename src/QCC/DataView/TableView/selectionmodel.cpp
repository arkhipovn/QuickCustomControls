#include "selectionmodel.h"

#include <QItemSelection>
#include <QGuiApplication>
#include <QClipboard>
#include <QDateTime>
#include <QTextStream>

#ifdef USING_WIDGETS
#include <QFileDialog>

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#endif

SelectionModel::SelectionModel(QAbstractItemModel *model, QObject *parent) : QItemSelectionModel(model, parent)
{
    selectionBehavior = SelectionBehavior::SelectItems;
    selectionMode = MultiSelection;
}

void SelectionModel::select(const QModelIndex &index, bool clear)
{
    if(!index.isValid()) return;
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..

    switch (selectionBehavior) {
        case SelectionBehavior::SelectItems:
            if(clear) QItemSelectionModel::select(index, QItemSelectionModel::ClearAndSelect);
            else QItemSelectionModel::select(index, QItemSelectionModel::Select);
            break;
        case SelectionBehavior::SelectRows:
            selectRow(index.row(), clear);
            break;
        case SelectionBehavior::SelectColumns:
            selectColumn(index.column(), clear);
            break;
        default: break;
    }
}

void SelectionModel::setCurrentIndex(const QModelIndex &index, bool clear)
{
    if(!index.isValid()) return;
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..

    if(clear) QItemSelectionModel::setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    else QItemSelectionModel::setCurrentIndex(index, QItemSelectionModel::Select);

    switch (selectionBehavior) {
        case SelectionBehavior::SelectItems:
            break;
        case SelectionBehavior::SelectRows:
            selectRow(index.row(), clear);
            break;
        case SelectionBehavior::SelectColumns:
            selectColumn(index.column(), clear);
            break;
        default: break;
    }
}

void SelectionModel::selection(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) {
        if(!bottomRight.isValid()) return;
        select(bottomRight);    // только одну ячейку выделим..
    }
    else {
        if(!topLeft.isValid() || !bottomRight.isValid()) return;
        QItemSelection itemSelection(topLeft, bottomRight);

        switch (selectionBehavior) {
            case SelectionBehavior::SelectItems:
                QItemSelectionModel::select(itemSelection, QItemSelectionModel::ToggleCurrent);
                break;
            case SelectionBehavior::SelectRows:
                QItemSelectionModel::select(itemSelection, QItemSelectionModel::ToggleCurrent | QItemSelectionModel::Rows);
                break;
            case SelectionBehavior::SelectColumns:
                QItemSelectionModel::select(itemSelection, QItemSelectionModel::ToggleCurrent | QItemSelectionModel::Columns);
                break;
            default: break;
        }
    }
}

void SelectionModel::selectRow(int row, bool clear)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..
    auto index = model()->index(row, 0);
    if(!index.isValid()) return;

    switch (selectionBehavior) {
        case SelectionBehavior::SelectItems:
            //            select(index, clear);
            //            break;
        case SelectionBehavior::SelectRows:
            if(clear) QItemSelectionModel::select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            else QItemSelectionModel::select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            break;
        default: break;
    }
}

void SelectionModel::selectColumn(int column, bool clear)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..
    auto index = model()->index(0, column);
    if(!index.isValid()) return;

    switch (selectionBehavior) {
        case SelectionBehavior::SelectItems:
            //            select(index, clear);
            //            break;
        case SelectionBehavior::SelectColumns:
            if(clear) QItemSelectionModel::select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Columns);
            else QItemSelectionModel::select(index, QItemSelectionModel::Select | QItemSelectionModel::Columns);
            break;
        default: break;
    }
}

void SelectionModel::selectAll()
{
    if(selectionMode == NoSelection) return;
    if(selectionMode == SingleSelection) return;
    if(model() == nullptr) return;

    QItemSelection itemSelection(model()->index(0, 0), model()->index(model()->rowCount() - 1, model()->columnCount() - 1));
    QItemSelectionModel::select(itemSelection, QItemSelectionModel::ClearAndSelect);
}

void SelectionModel::increaseRowIndex(bool clear)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..

    //    auto index = model()->index(currentIndex().row() + 1, currentIndex().column());
    //    if(!index.isValid()) return;
    //    setCurrentIndex(index, clear);

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    auto index = model()->index(currentIndex().row() + 1, currentIndex().column());   //@N проверить
#else
    auto index = currentIndex().siblingAtRow(currentIndex().row() + 1);
#endif

    if(!index.isValid()) return;
    setCurrentIndex(index, clear);
}

void SelectionModel::decreaseRowIndex(bool clear)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    auto index = model()->index(currentIndex().row() - 1, currentIndex().column());   //@N проверить
#else
    auto index = currentIndex().siblingAtRow(currentIndex().row() - 1);
#endif

    if(!index.isValid()) return;
    setCurrentIndex(index, clear);
}

void SelectionModel::increaseColumnIndex(bool clear)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    auto index = model()->index(currentIndex().row(), currentIndex().column() + 1);   //@N проверить
#else
    auto index = currentIndex().siblingAtColumn(currentIndex().column() + 1);
#endif


    if(!index.isValid()) return;
    setCurrentIndex(index, clear);
}

void SelectionModel::decreaseColumnIndex(bool clear)
{
    if(selectionMode == NoSelection) return;
    if(model() == nullptr) return;
    if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    auto index = model()->index(currentIndex().row(), currentIndex().column() - 1);   //@N проверить
#else
    auto index = currentIndex().siblingAtColumn(currentIndex().column() - 1);
#endif

    if(!index.isValid()) return;
    setCurrentIndex(index, clear);
}

QJSValue SelectionModel::getRoleProvider() const
{
    return roleProvider;
}

void SelectionModel::setRoleProvider(const QJSValue &value)
{
    roleProvider = value;
    emit roleProviderChanged();
}

SelectionModel::SelectionMode SelectionModel::getSelectionMode() const
{
    return selectionMode;
}

void SelectionModel::setSelectionMode(const SelectionModel::SelectionMode &value)
{
    if(selectionMode != value) {
        selectionMode = value;
        if(selectionMode == NoSelection)
            clearSelection();
        //        if(selectionMode == SingleSelection) clear = true;  // сбрасываем, если нужно выделить только один элемент..
        emit selectionModeChanged();
    }
}

SelectionModel::SelectionBehavior SelectionModel::getSelectionBehavior() const
{
    return selectionBehavior;
}

void SelectionModel::setSelectionBehavior(const SelectionModel::SelectionBehavior &value)
{
    if(selectionBehavior != value) {
        selectionBehavior = value;
        emit selectionBehaviorChanged();
    }
}

void SelectionModel::copy(const QStringList &verticalHeader, const QStringList &horizontalHeader)
{
    auto list = getSelectedText("\t", verticalHeader, horizontalHeader);
    QGuiApplication::clipboard()->setText(list.join("\n") + "\n");
}


#ifdef USING_WIDGETS
void SelectionModel::saveToCSV(const QStringList &verticalHeader, const QStringList &horizontalHeader, bool fullTable)
{
    QString filters = "*.csv;";
    QString defaultFilter("*.csv");

    QString name = QFileDialog::getSaveFileName(0, tr("Save file"), QDir::currentPath(),
                                                filters, &defaultFilter);
    if(name.isEmpty()) return;
    saveToCSV(name, verticalHeader, horizontalHeader, fullTable);
}

void SelectionModel::printTable(const QStringList &verticalHeader, const QStringList &horizontalHeader, bool fullTable)
{
    QString text = getSelectedText("\n", verticalHeader, horizontalHeader, fullTable).join("\n");
    QPrinter printer;
    QPrintDialog printDialog(&printer);
    printDialog.setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); //@N
    printDialog.setWindowTitle("Print Document");

    auto exec = printDialog.exec();
    if(exec == QDialog::Accepted) {
        QPainter painter(&printer);

        painter.drawText(painter.viewport(), Qt::AlignLeft|Qt::AlignTop, text);
        painter.end();
    }
}
#endif


void SelectionModel::saveToCSV(const QString &file, const QStringList &verticalHeader, const QStringList &horizontalHeader, bool fullTable)
{
    QFile f(file);
    if(f.open(QIODevice::WriteOnly)) {
        QTextStream ts(&f);
        auto list = getSelectedText(";", verticalHeader, horizontalHeader, fullTable);
        for(auto l : list) {
            ts << l + QLatin1Char('\n');
        }
        f.close();
    }
}

//QStringList SelectionModel::getSelectedItems(const QString &sep)
//{
//    if(model() == nullptr) return {};
//    auto cells = selectedIndexes();
//}

//QStringList SelectionModel::getSelectedRows(const QString &sep)
//{
//    if(model() == nullptr) return {};
//    auto cells = selectedIndexes();
//}

//QStringList SelectionModel::getSelectedColumns(const QString &sep)
//{
//    if(model() == nullptr) return {};
//    auto cells = selectedIndexes();
//}

QStringList SelectionModel::getSelectedText(const QString &sep, const QStringList &verticalHeader, const QStringList &horizontalHeader, bool fullTable)
{
    if(model() == nullptr) return {};
    QStringList selectedText;

    // Найдем роль для каждого столбца
    auto roleNames = model()->roleNames();

    int role = Qt::DisplayRole;
    if(roleNames.size() == 1) {
        role = roleNames.begin().key();
    }

    QVector <int> roles(model()->columnCount(), role);
    if(roleNames.size() > 1) {
        for(int c = 0; c < model()->columnCount(); ++c) {
            if(!roleProvider.isUndefined() && roleProvider.isCallable()) {
                auto const rowAsArgument = QJSValueList() << QJSValue(c);
                auto array = roleProvider.call(rowAsArgument).toString().toLatin1();    // нашли строковое представление роли..
                // найдем числовое

                int role = roleNames.key(array);
                roles[c] = role;
            }
        }
    }

    if(fullTable) {
        QString line;

        if(!horizontalHeader.isEmpty()) {
            if(!verticalHeader.isEmpty()) line += sep;
            for(int c = 0; c < model()->columnCount(); ++c) {
                if(c < horizontalHeader.size())
                    line += horizontalHeader.at(c) + sep;
                else line += sep;
            }
            selectedText.append(line);
            line.clear();
        }

        for(int r = 0; r < model()->rowCount(); ++r) {
            line.clear();
            if(!verticalHeader.isEmpty()) {
                if(r < verticalHeader.size())
                    line += verticalHeader.at(r) + sep;
                else  line += sep;
            }

            for(int c = 0; c < model()->columnCount(); ++c) {
                QModelIndex index = model()->index(r, c);
                auto data = model()->data(index, roles.at(c));
                if(data.type() == QVariant::DateTime)
                    line += data.toDateTime().toString("dd.MM.yyyy hh:mm:ss") + sep;  //@N
                else line += data.toString() + sep;
            }
            selectedText.append(line);
        }
    }
    else {
        if(selectionMode == NoSelection) return {};
        // потом разделить..
        //    switch (selectionBehavior) {
        //        case SelectItems: return getSelectedItems(sep);
        //        case SelectRows: return getSelectedRows(sep);
        //        case SelectColumns: return getSelectedColumns(sep);
        //        default:
        //            break;
        //    }
        //    return {};

        auto cells = selectedIndexes();
        if(cells.isEmpty()) return {};
        std::sort(cells.begin(), cells.end());

        QList <int> columns;
        for(auto c : cells) {
            if(columns.contains(c.column())) continue;
            columns.append(c.column());
        }
        if(columns.isEmpty()) return {};
        std::sort(columns.begin(), columns.end());
        int minCol = columns.first();

        int lastRow = cells.first().row();
        int lastColumn = minCol - 1; //-1
        bool rowheaderwrited = false;

        if(!horizontalHeader.isEmpty()) {
            QString line;
            if(!verticalHeader.isEmpty()) line += sep;
            for(auto c : columns) {
                if(c < horizontalHeader.size())
                    line += horizontalHeader.at(c) + sep;
                else line += sep;
            }
            selectedText.append(line);
        }

        QStringList list;
        for(auto c : cells) {
            if(c.row() != lastRow) {
                selectedText.append(list.join(sep));
                list.clear();
                lastColumn = minCol - 1; // -1
                lastRow = c.row();
                rowheaderwrited = false;
            }

            if(c.row() == lastRow && !rowheaderwrited) {
                if(!verticalHeader.isEmpty()) {
                    if(c.row() < verticalHeader.size())
                        list << verticalHeader.at(c.row());
                    else  list << "";   //@N
                }
                rowheaderwrited = true;
            }

            if(c.column() != lastColumn) {
                while(++lastColumn < c.column()) {
                    if(columns.contains(lastColumn)) list << " ";
                }
                lastColumn = c.column(); // навсякий..
            }

            auto data = c.data(roles.at(c.column()));
            if(data.type() == QVariant::DateTime)
                list << data.toDateTime().toString("dd.MM.yyyy hh:mm:ss");  //@N
            else list << data.toString();
        }

        if(!list.isEmpty()) selectedText.append(list.join(sep));
    }

    return selectedText;
}
