#include "xymapper.h"
#include <QDateTime>
#include <QDate>
#include <QDebug>

QT_CHARTS_BEGIN_NAMESPACE

XYMapper::XYMapper(QObject *parent) : QObject(parent), model_(nullptr), series_(nullptr),
    seriesSignalBlocked_(false), orientation_(Qt::Vertical), xSection_(0), ySection_(1), first_(0), count_(-1), seriesNameSection_(-1)
{

}

XYMapper::~XYMapper()
{

}

QAbstractItemModel *XYMapper::model() const
{
    return model_;
}

void XYMapper::setModel(QAbstractItemModel *model)
{
    if(model_ == model)
        return;

    if(model_ != nullptr)
        disconnect(model_, 0, this, 0);

    model_ = model;
    emit modelChanged();

    if(model != nullptr) {
        QObject::connect(model_, &QAbstractItemModel::dataChanged, this, &XYMapper::dataChanged);
        QObject::connect(model_, &QAbstractItemModel::rowsInserted, this, &XYMapper::rowsInserted);
        QObject::connect(model_, &QAbstractItemModel::rowsRemoved, this, &XYMapper::rowsRemoved);
        QObject::connect(model_, &QAbstractItemModel::columnsInserted, this, &XYMapper::columnsInserted);
        QObject::connect(model_, &QAbstractItemModel::columnsRemoved, this, &XYMapper::columnsRemoved);

        QObject::connect(model_, &QAbstractItemModel::destroyed, this, &XYMapper::modelDestroyed);
        QObject::connect(model_, &QAbstractItemModel::layoutChanged, this, &XYMapper::initializeDataFromModel);
        QObject::connect(model_, &QAbstractItemModel::modelReset, this, &XYMapper::initializeDataFromModel);

        QObject::connect(model_, &QAbstractItemModel::headerDataChanged, this, &XYMapper::headerDataChanged);
    }

    initializeSeriesNameFromModel();
    initializeDataFromModel();
}

QXYSeries *XYMapper::series() const
{
    return series_;
}

void XYMapper::setSeries(QXYSeries *series)
{
    if(series_ == series)
        return;

    if(series_ != nullptr)
        disconnect(series_, 0, this, 0);

    series_ = series;
    emit seriesChanged();

    if(series == nullptr)   // в этом случае обновлять нечего..
        return;

    QObject::connect(series_, &QXYSeries::destroyed, this, &XYMapper::seriesDestroyed);

    initializeSeriesNameFromModel();
    initializeDataFromModel();
}

void XYMapper::blockSeriesSignals(bool b)
{
    seriesSignalBlocked_ = b;
}

bool XYMapper::seriesSignalBlocked() const
{
    return seriesSignalBlocked_;
}

qreal XYMapper::valueFromModel(const QModelIndex &index) const
{
    QVariant value = model_->data(index);   //@N role

    switch (value.type()) {
        case QVariant::Date:
            return QDateTime(value.toDate()).toMSecsSinceEpoch();
        case QVariant::DateTime:
            return value.toDateTime().toMSecsSinceEpoch();
        default:
            return value.toReal();
    }
}

int XYMapper::last() const
{
    int c = getModelSize();
    return count_ < 0 ? c
                      : first_ + count_ >= c ? c
                                             : first_ + count_;
}

int XYMapper::getModelSize() const
{
    return orientation_ == Qt::Vertical ? model_->rowCount()
                                        : model_->columnCount();
}

int XYMapper::count() const
{
    return count_;
}

void XYMapper::setCount(int count)
{
    if(count_ == count)
        return;
    count_ = std::max(-1, count); // -1 - это максимальный размер
    emit countChanged();
    initializeDataFromModel();
}

int XYMapper::first() const
{
    return first_;
}

void XYMapper::setFirst(int first)
{
    if(first_ == first)
        return;
    first_ = std::max(0, first);
    emit firstChanged();
    initializeDataFromModel();
}

int XYMapper::ySection() const
{
    return ySection_;
}

void XYMapper::setYSection(int ySection)
{
    if(ySection_ == ySection)
        return;
    ySection_ = ySection;
    emit ySectionChanged();
    initializeDataFromModel();
}

int XYMapper::xSection() const
{
    return xSection_;
}

void XYMapper::setXSection(int xSection)
{
    if(xSection_ == xSection)
        return;
    xSection_ = xSection;
    emit xSectionChanged();
    initializeDataFromModel();
}

int XYMapper::seriesNameSection() const
{
    return seriesNameSection_;
}

void XYMapper::setSeriesNameSection(int seriesNameSection)
{
    if(seriesNameSection_ == seriesNameSection)
        return;
    seriesNameSection_ = seriesNameSection;
    emit seriesNameSectionChanged();
    if(seriesNameSection_ != -1)
        initializeSeriesNameFromModel();
}

void XYMapper::seriesDestroyed()
{
    series_ = nullptr;
}

void XYMapper::modelDestroyed()
{
    model_ = nullptr;
    initializeDataFromModel();
}

void XYMapper::initializeSeriesNameFromModel()
{
    if(series_ == nullptr || model_ == nullptr)
        return;
    if(seriesNameSection_ < 0 || seriesNameSection_ >= getModelSize())
        return;
    series_->setName(model_->headerData(seriesNameSection_, orientation_ == Qt::Vertical ? Qt::Horizontal
                                                                                         : Qt::Vertical).toString()); //@N role
}

void XYMapper::initializeDataFromModel()
{
    if(series_ == nullptr)
        return;

    QVector<QPointF> points;

    blockSeriesSignals(true);

    series_->clear();

    if(model_ == nullptr) {
        series_->replace(points);
        return;
    }

    int columns = model_->columnCount();

    if(xSection_ < 0 || xSection_ >= columns ||
            ySection_ < 0 || ySection_ >= columns) {
        series_->replace(points);
        return;
    }

    int lastRow = last();

    for(int r = first_; r < lastRow; ++r) {
        auto xIndex = model_->index(r, xSection_);
        if(!xIndex.isValid()) break; // continue; //@N я думаю, что продолжать не имеет смысла

        auto yIndex = model_->index(r, ySection_);
        if(!yIndex.isValid()) break; // continue; //@N я думаю, что продолжать не имеет смысла

        points.append({ valueFromModel(xIndex), valueFromModel(yIndex) });
    }

    qDebug() << "replase" << series_;
    series_->replace(points);
    blockSeriesSignals(false);
}

void XYMapper::replace(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{

}

void XYMapper::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if(seriesSignalBlocked())
        return;

    if(!roles.contains(Qt::DisplayRole))    //@N role
        return;

    if(first_ > bottomRight.row())
        return;

    int lastRow = last();

    if(lastRow < topLeft.row())
        return;

    if(xSection_ > bottomRight.column() && ySection_ > bottomRight.column())
        return;

    if(xSection_ < topLeft.column() && ySection_ < topLeft.column())
        return;

    initializeDataFromModel();
}

void XYMapper::rowsInserted(const QModelIndex &parent, int start, int end)
{
    if(seriesSignalBlocked())
        return;
    Q_UNUSED(parent)
    Q_UNUSED(end)

    if(start <= first_ || start <= last())  //@N поправить
        initializeDataFromModel();


    /*
    //    blockSeriesSignals(true);
    //    if(count_ != -1 && start >= first_ + count_) {

    //    }
    for(int r = first_; r < lastRow; ++r) {
        auto xIndex = model_->index(r, xSection_);
        if(!xIndex.isValid()) break; // continue; //@N я думаю, что продолжать не имеет смысла

        auto yIndex = model_->index(r, ySection_);
        if(!yIndex.isValid()) break; // continue; //@N я думаю, что продолжать не имеет смысла

        points.append({ valueFromModel(xIndex), valueFromModel(yIndex) });
    }

    if(first_ > bottomRight.row())
        return;

    int lastRow = last();

    if(lastRow < topLeft.row())
        return;

    if(xSection_ > bottomRight.column() && ySection_ > bottomRight.column())
        return;

    if(xSection_ < topLeft.column() && ySection_ < topLeft.column())
        return;

        series_->append();

    int lastRow = last();
    if((start >= first_ && start < lastRow) ||
            (end >= first_ && end < lastRow)) {
        int removedFirst = start - first_;
        int removedCount = end - start + 1;
        series_->remove(removedFirst, removedCount);
    }

    blockSeriesSignals(false);
    */

}

void XYMapper::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    if(seriesSignalBlocked())
        return;

    if(series_ == nullptr) return;
    if(series_->count() < start) return;

    blockSeriesSignals(true);

    int lastRow = last();
    if((start >= first_ && start < lastRow) ||
            (end >= first_ && end < lastRow)) {
        int removedFirst = start - first_;
        int removedCount = end - start + 1;
        series_->remove(removedFirst, removedCount);
    }

    blockSeriesSignals(false);
    //            initializeDataFromModel();
}

void XYMapper::columnsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(end)

    if(seriesSignalBlocked())
        return;

    if(start <= xSection_ || start <= ySection_)    //@N поправить
        initializeDataFromModel();
}

void XYMapper::columnsRemoved(const QModelIndex &parent, int start, int end)
{
    if(seriesSignalBlocked())
        return;
    Q_UNUSED(parent)
    Q_UNUSED(end)
    if(start <= xSection_ || start <= ySection_)    //@N поправить
        initializeDataFromModel();
}

void XYMapper::headerDataChanged(Qt::Orientation orientation, int start, int end)
{
    if(series_ == nullptr || seriesNameSection_ == -1)
        return;
    if(orientation != orientation_) return;
    if(start <= seriesNameSection_ && end >= seriesNameSection_)
        initializeSeriesNameFromModel();
}

QT_CHARTS_END_NAMESPACE
