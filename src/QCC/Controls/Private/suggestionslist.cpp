#include "suggestionslist.h"
#include <QDebug>

SuggestionsList::SuggestionsList(QObject *parent) : QAbstractListModel(parent)
{
    sensitivity = Qt::CaseInsensitive;
    maximumNumberOfSuggestions = 100;
}

int SuggestionsList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return suggestions.size();
}

QVariant SuggestionsList::data(const QModelIndex &index, int role) const
{
    if(index.row() >= suggestions.size()) return QVariant();
    switch (role) {
        case SuggestionModelDataRole: return suggestions.at(index.row()).suggestionModelData;
        case SuggestionRole: return suggestions.at(index.row()).suggestion;
        case SuggestionIndexRole: return suggestions.at(index.row()).index;
        default: return QVariant();
    }
}

QHash<int, QByteArray> SuggestionsList::roleNames() const
{
    static const QHash <int, QByteArray> roles = QHash <int, QByteArray> {
        { Roles::SuggestionRole, "suggestion" },
        { Roles::SuggestionIndexRole, "suggestionIndex" },
        { Roles::SuggestionModelDataRole, "suggestionModelData" }
    };
    return roles;
}

void SuggestionsList::setText(const QString &value)
{
    if(text != value) {
        text = value;
        emit textChanged();

        setSuggestions(searchMatches(value, model));
    }
}

void SuggestionsList::setModel(const QStringList &value)
{
    if(model != value) {
        model = value;
        model.removeDuplicates();
        emit modelChanged();

        setSuggestions(searchMatches(text, value));
    }
}

void SuggestionsList::setMaximumNumberOfSuggestions(int value)
{
    if(maximumNumberOfSuggestions != value && value > 0) {
        maximumNumberOfSuggestions = value;
        emit maximumNumberOfSuggestionsChanged();

        setSuggestions(searchMatches(text, model));
    }
}

void SuggestionsList::setSensitivity(const Qt::CaseSensitivity &value)
{
    if(sensitivity != value) {
        sensitivity = value;
        emit sensitivityChanged();
        setSuggestions(searchMatches(text, model));
    }
}

void SuggestionsList::setSuggestions(const QList<Suggestion> &suggestions)
{
    if(!this->suggestions.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, this->suggestions.size() - 1);
        //    beginResetModel();
        this->suggestions.clear();
        endRemoveRows();
        //    endResetModel();
    }

    if(suggestions.isEmpty()) return;
    beginInsertRows(QModelIndex(), 0, suggestions.size() - 1);
    this->suggestions = suggestions;
    endInsertRows();
    //    emit dataChanged(QModelIndex(0, 0), QModelIndex(suggestions.size() - 1, 0));
}

QList <SuggestionsList::Suggestion> SuggestionsList::searchMatches(const QString &text, const QStringList &model) const
{
    if(text.isEmpty()) return {}; //@N ? list = model;
    QList <Suggestion> list;
    for(int i = 0; i < model.size(); ++i) {
        if(list.size() >= maximumNumberOfSuggestions) break;

        auto indexOf = model.at(i).indexOf(text, 0, sensitivity);
        if(indexOf < 0) continue;

        // похоже полное сопадение
        if(model.at(i).size() == text.size()) continue;

        QString str = model.at(i);
        str.insert(indexOf + text.size(), "</b>");
        str.insert(indexOf, "<b>");
        list.append({model.at(i), str, i});
    }
    return list;
}


QString SuggestionsList::getText() const { return text; }
QStringList SuggestionsList::getModel() const { return model; }
int SuggestionsList::getMaximumNumberOfSuggestions() const { return maximumNumberOfSuggestions; }
Qt::CaseSensitivity SuggestionsList::getSensitivity() const { return sensitivity; }

void SuggestionsList::clear()
{
    text.clear();
    model.clear();

    if(this->suggestions.isEmpty()) return;
    beginRemoveRows(QModelIndex(), 0, this->suggestions.size() - 1);
    //    beginResetModel();
    this->suggestions.clear();
    endRemoveRows();
    //    endResetModel();
}




