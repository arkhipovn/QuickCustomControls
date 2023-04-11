#ifndef SUGGESTIONSLIST_H
#define SUGGESTIONSLIST_H

#include <QAbstractListModel>
#include <QStringList>

class SuggestionsList : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QStringList model READ getModel WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int maximumNumberOfSuggestions READ getMaximumNumberOfSuggestions WRITE setMaximumNumberOfSuggestions NOTIFY maximumNumberOfSuggestionsChanged)
    Q_PROPERTY(Qt::CaseSensitivity sensitivity READ getSensitivity WRITE setSensitivity NOTIFY sensitivityChanged)

public:
    explicit SuggestionsList(QObject *parent = nullptr);

    enum Roles {
        SuggestionRole = Qt::UserRole + 1,
        SuggestionModelDataRole,
        SuggestionIndexRole
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    struct Suggestion
    {
        QString suggestionModelData;    // оригинальный текст
        QString suggestion;             // предложение. Это оригинальный текст + теги <b> для выделения совпадения
        int index;                      // индексы текущий предложений из model
    };

    QStringList getModel() const;
    void setModel(const QStringList &value);

    QString getText() const;
    void setText(const QString &value);

    int getMaximumNumberOfSuggestions() const;
    void setMaximumNumberOfSuggestions(int value);

    Qt::CaseSensitivity getSensitivity() const;
    void setSensitivity(const Qt::CaseSensitivity &value);

public slots:
    void clear();

signals:
    void textChanged();
    void modelChanged();
    //    void suggestionsChanged();
    void maximumNumberOfSuggestionsChanged();
    void sensitivityChanged();

private:
    Qt::CaseSensitivity sensitivity;
    int maximumNumberOfSuggestions;
    QString text;
    QStringList model;

    QList <Suggestion> suggestions;


    void setSuggestions(const QList <Suggestion> &suggestions);
    QList <SuggestionsList::Suggestion> searchMatches(const QString &text, const QStringList &model) const;
};

#endif // SUGGESTIONSLIST_H
