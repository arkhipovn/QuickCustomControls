#include "tooltiplistmodel.h"
#include <QtMath>
#include <QDebug>

ToolTipListModel::ToolTipListModel(QObject *parent) : QAbstractListModel(parent),
    listType_(ListType::All),
    timeout_(1000)
{
    timer_ = new QTimer(this);
    setTimerInterval();
    QObject::connect(timer_, &QTimer::timeout, this, &ToolTipListModel::checkList);
}

QHash<int, QByteArray> ToolTipListModel::roleNames() const
{
    static const QHash <int, QByteArray> roles = QHash <int, QByteArray> {
        { Qt::DisplayRole, "display" },
        { Qt::BackgroundRole, "backgroundColor" },
        { Qt::ForegroundRole, "foregroundColor" }
    };
    return roles;
}

QVariant ToolTipListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= list_.size())
        return QVariant();
    switch (role) {
        case Qt::DisplayRole: return list_.at(index.row()).text;
        case Qt::BackgroundRole: return list_.at(index.row()).background;
        case Qt::ForegroundRole: return list_.at(index.row()).foreground;
        default: break;
    }

    return QVariant();
}

int ToolTipListModel::rowCount(const QModelIndex &) const
{
    return list_.size();
}

void ToolTipListModel::append(const QString &text, const QColor &background, const QColor &foreground)
{
    switch (listType_) {
        case ListType::LatterDoNotRepeat:
            if(list_.size() > 0) {      // Сначала хотел, чтобы вообще не было повторений.
                if(list_.last().text == text) {
                    return;
                }
            }
            break;
        case ListType::NoRepeats:
            if(indexOf(text) != -1)
                return;
            break;
        default:
            break;
    }

    beginInsertRows(QModelIndex(), list_.size(), list_.size());
    Message m;
    m.text = text;
    m.background = background;
    m.foreground = foreground;
    m.timer.start();
    list_.append(m);
    endInsertRows();

    if(!timer_->isActive())
        timer_->start();
}

void ToolTipListModel::removeAt(int i)
{
    if(i < 0 || i >= list_.size())
        return;
    beginRemoveRows(QModelIndex(), i, i);
    list_.removeAt(i);
    endRemoveRows();

    if(list_.isEmpty())
        timer_->stop();
}

void ToolTipListModel::clear()
{
    if(list_.isEmpty())
        return;

    list_.clear();
    beginRemoveRows(QModelIndex(), 0, list_.size() - 1);
    list_.clear();
    endRemoveRows();
}

void ToolTipListModel::checkList()
{
    for(int i = 0; i < list_.size(); ++i) {
        if(list_.at(i).timer.elapsed() > timeout_) {
            removeAt(i);
            --i;
        }
    }
}

ToolTipListModel::ListType ToolTipListModel::listType() const
{
    return listType_;
}

void ToolTipListModel::setListType(const ListType &listType)
{
    if(listType_ != listType) {
        listType_ = listType;
        emit listTypeChanged();
    }
}

void ToolTipListModel::setTimerInterval()
{
    timer_->setInterval(std::min(std::min(static_cast<double>(timeout_), (timeout_ / 5.)), 100.));
}

int ToolTipListModel::indexOf(const QString &text) const
{
    for(int i = 0; i < list_.size(); ++i) {
        if(list_.at(i).text == text) {
            return i;
        }
    }
    return -1;
}

int ToolTipListModel::timeout() const
{
    return timeout_;
}

void ToolTipListModel::setTimeout(int value)
{
    if(timeout_ != value) {
        timeout_ = value;
        setTimerInterval();
        emit timeoutChanged();
    }
}
