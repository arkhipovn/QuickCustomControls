#ifndef TOOLTIPLISTMODEL_H
#define TOOLTIPLISTMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QElapsedTimer>
#include <QTimer>

class ToolTipListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(ListType listType READ listType WRITE setListType NOTIFY listTypeChanged)

public:
    explicit ToolTipListModel(QObject *parent = nullptr);

    enum class ListType {
        All,
        LatterDoNotRepeat,
        NoRepeats
    };

    Q_ENUMS(ListType)

    struct Message
    {
        QString text;
        QColor background;
        QColor foreground;
        QElapsedTimer timer;
    };

    QHash <int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex & = QModelIndex()) const;

    Q_INVOKABLE void append(const QString &text,
                            const QColor &background = Qt::white,
                            const QColor &foreground = Qt::black);
    Q_INVOKABLE void removeAt(int i);
    Q_INVOKABLE void clear();

    int timeout() const;
    void setTimeout(int value);

    ListType listType() const;
    void setListType(const ListType &listType);

signals:
    void timeoutChanged();
    void listTypeChanged();

private slots:
    void checkList();

private:
    ListType listType_;
    int timeout_;
    QTimer *timer_;
    QList <Message> list_;

    void setTimerInterval();
    int indexOf(const QString &text) const; // Поиск совпадений
};

#endif // TOOLTIPLISTMODEL_H
