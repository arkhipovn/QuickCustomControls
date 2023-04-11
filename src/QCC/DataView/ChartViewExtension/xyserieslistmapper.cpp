#include "xyserieslistmapper.h"
#include <math.h>
#include <QDateTime>
#include <QElapsedTimer>
#include <QDebug>

QT_CHARTS_BEGIN_NAMESPACE

XYSeriesListMapper::XYSeriesListMapper(QObject *parent) : QObject(parent),
    model_(nullptr),
    xSectionRole_(Qt::DisplayRole),
    ySectionRole_(Qt::DisplayRole),
    seriesNameSection_(1),
    averagingInterval_(1),
    completed_(false),
    seriesSignalBlocked_(false)
{
    domain_ = new XYDomain(this);
    //    setXSectionProvider(QJSValue([](int index){
    //        return index * 2;
    //    }));
    //    ySectionProvider_ = QJSValue([](int index){
    //        return index * 2 + 1;
    //    });
}

XYSeriesListMapper::~XYSeriesListMapper()
{
    //    qDebug() << "~XYSeriesListMapper";
}

void XYSeriesListMapper::classBegin() {}

void XYSeriesListMapper::componentComplete()
{
    completed_ = true;
    //    emit updateSeriesList();
}

QAbstractItemModel *XYSeriesListMapper::model() const
{
    return model_;
}

void XYSeriesListMapper::setModel(QAbstractItemModel *model)
{
    if(model_ == model)
        return;

    if(model_ != nullptr)
        disconnect(model_, 0, this, 0);

    domain_->reset();
    model_ = model;

    emit modelChanged();

    if(model != nullptr) {
        QObject::connect(model_, &QAbstractItemModel::columnsInserted, this, &XYSeriesListMapper::columnsInserted);
        QObject::connect(model_, &QAbstractItemModel::columnsRemoved, this, &XYSeriesListMapper::columnsRemoved);

        QObject::connect(model_, &QAbstractItemModel::dataChanged, this, &XYSeriesListMapper::dataChanged);
        QObject::connect(model_, &QAbstractItemModel::rowsInserted, this, &XYSeriesListMapper::rowsInserted);
        QObject::connect(model_, &QAbstractItemModel::rowsRemoved, this, &XYSeriesListMapper::rowsRemoved);

        QObject::connect(model_, &QAbstractItemModel::destroyed, this, &XYSeriesListMapper::modelDestroyed);


        QObject::connect(model_, &QAbstractItemModel::layoutChanged, this, &XYSeriesListMapper::updateSeriesList);
        QObject::connect(model_, &QAbstractItemModel::modelReset, this, &XYSeriesListMapper::reset);

        QObject::connect(model_, &QAbstractItemModel::headerDataChanged, this, &XYSeriesListMapper::headerDataChanged);
    }

    if(isCompleted()) emit updateSeriesList();
}

void XYSeriesListMapper::initializeSeriesNameFromModel(int index)
{
    if(index < 0 || index >= seriesList_.size())
        return;
    if(model_ == nullptr) return;
    int section = seriesNameSection_ == 1 ? getYSection(index)
                                          : getXSection(index);
    seriesList_[index]->setName(model_->headerData(section, Qt::Horizontal).toString());
}

void XYSeriesListMapper::initializeSeriesFromModel(int index)
{
    qInfo() << "initializeSeriesFromModel" << index;
    if(index < 0 || index >= seriesList_.size())
        return;

    QXYSeries *xySeries = static_cast<QXYSeries *>(seriesList_.at(index));

    if(model_ == nullptr) {
        xySeries->clear();
        domainList_[index]->reset();
        return;
    }

    int columns = model_->columnCount();
    int rows = model_->rowCount();
    int xSection = getXSection(index);
    int ySection = getYSection(index);
    qInfo() << "init" << columns << rows << xSection << ySection;

    if(columns <= xSection || columns <= ySection || rows == 0) {
        xySeries->clear();
        domainList_[index]->reset();
        return;
    }

    bool isok = true;
    qreal x = valueFromModel(0, xSection, xSectionRole_ &isok);
    if(!isok) {
        xySeries->clear();
        domainList_[index]->reset();
        return;
    }

    qreal y = valueFromModel(0, ySection, ySectionRole_, &isok);
    if(!isok) {
        xySeries->clear();
        domainList_[index]->reset();
        return;
    }

    qreal maxX = x;
    qreal minX = x;
    qreal maxY = y;
    qreal minY = y;

    QList <QPointF> points = { { x, y } };

    // Пройдемся по всем остальным

    if(averagingInterval_ == 1) {
        for(int i = 1; i < rows; ++i) {
            x = valueFromModel(i, xSection, xSectionRole_, &isok);
            if(!isok) break;            // Думаю продолжать не имеет смысла

            y = valueFromModel(i, ySection, ySectionRole_, &isok);
            if(!isok) break;            // Думаю продолжать не имеет смысла

            maxX = std::max(x, maxX);
            minX = std::min(x, minX);

            maxY = std::max(y, maxY);
            minY = std::min(y, minY);

            points.append({ x, y });
        }
    }
    else {
        qreal last = 0;
        qreal start = x;
        qreal sum = y;
        int count = 1;

        for(int i = 1; i < rows; ++i) {
            x = valueFromModel(i, xSection, xSectionRole_, &isok);
            if(!isok) break;            // Думаю продолжать не имеет смысла

            y = valueFromModel(i, ySection, ySectionRole_, &isok);
            if(!isok) break;            // Думаю продолжать не имеет смысла

            if(x - start < averagingInterval_) {
                // меньше времени, значит добавляем в сумму и увеличиваем счетчик
                sum += y;
                ++count;
            }
            else {
                // находим среднее и добавляем в массив
                if(count > 0) {
                    qreal aveX = last > 0 ? (start + last) / 2.
                                          : start;
                    qreal aveY = sum / static_cast<double>(count);

                    maxX = std::max(aveX, maxX);
                    minX = std::min(aveX, minX);
                    maxY = std::max(aveY, maxY);
                    minY = std::min(aveY, minY);

                    points.append( { aveX, aveY } );
                }

                // начинаем заново
                start = x;
                sum = y;
                count = 1;
            }

            last = x;
        }

        // прошлись, теперь добавим последнюю точку, если она есть
        if(count > 0) {
            qreal aveX = last > 0 ? (start + last) / 2.
                                  : start;
            qreal aveY = sum / static_cast<double>(count);

            maxX = std::max(aveX, maxX);
            minX = std::min(aveX, minX);
            maxY = std::max(aveY, maxY);
            minY = std::min(aveY, minY);

            points.append( { aveX, aveY } );
        }
    }

    xySeries->replace(points);

    if(points.isEmpty())
        domainList_[index]->reset();
    else
        domainList_[index]->setRange(minX, maxX, minY, maxY);
}

void XYSeriesListMapper::append(QAbstractSeries *series)
{
    seriesList_.append(series);
    initializeSeriesNameFromModel(seriesList_.size() - 1);

    auto domain = new XYDomain(this/*series*/); // Почему-то не всегда вызывается удаление.. Нужно бы разобраться..
    QObject::connect(domain, &XYDomain::updated, this, &XYSeriesListMapper::updateDomain);

    domainList_.append(domain);
    initializeSeriesFromModel(seriesList_.size() - 1);
}

void XYSeriesListMapper::insert(int index, QAbstractSeries *series)
{
    seriesList_.insert(index, series);
    initializeSeriesNameFromModel(index);

    auto domain = new XYDomain(this/*series*/);  // Почему-то не всегда вызывается удаление.. Нужно бы разобраться..
    QObject::connect(domain, &XYDomain::updated, this, &XYSeriesListMapper::updateDomain);

    domainList_.insert(index, domain);
    initializeSeriesFromModel(index);
}

void XYSeriesListMapper::removeAt(int index)
{
    seriesList_.removeAt(index);
    domainList_[index]->deleteLater();
    domainList_.removeAt(index);
    updateDomain();
}

void XYSeriesListMapper::removeAll()
{
    if(!domainList_.isEmpty())
        domain_->reset();

    seriesList_.clear();

    for(int i = 0; i < domainList_.size(); ++i) {
        domainList_.at(i)->deleteLater();
    }

    domainList_.clear();
}

XYDomain *XYSeriesListMapper::domainAt(int index) const
{
    if(index < 0 || index >= domainList_.size()) return nullptr;
    return domainList_.at(index);
}

XYDomain *XYSeriesListMapper::domain() const
{
    return domain_;
}

void XYSeriesListMapper::headerDataChanged(Qt::Orientation orientation, int start, int end)
{
    if(orientation == Qt::Vertical) return;

    bool started = false;
    for(int i = 0; i <  model_->columnCount(); ++i) {    // Защита. Ну уж точно не мб быть больше чем columnCount..
        int section = seriesNameSection_ == 1 ? getYSection(i)
                                              : getXSection(i);

        if(section < 0) break;
        if(start >= section && section <= end) {
            initializeSeriesNameFromModel(i);
            started = true;
        }
        else if(started) break;
    }
}

qreal XYSeriesListMapper::valueFromModel(int row, int column, int role, bool *ok) const
{
    auto index = model_->index(row, column);
    if(!index.isValid()) {
        if(ok != nullptr) *ok = false;
        return 0;
    }
    return valueFromModel(index, role, ok);
}

qreal XYSeriesListMapper::valueFromModel(const QModelIndex &index, int role, bool *ok) const
{
    QVariant value = model_->data(index, role);   //@N role

    switch (value.type()) {
        case QVariant::Date: {
            if(ok != nullptr) *ok = false;
            auto date = value.toDate();
            if(!date.isValid())
                return 0;

            if(ok != nullptr) *ok = true;
            return QDateTime(date).toMSecsSinceEpoch();
        }
        case QVariant::DateTime: {
            if(ok != nullptr) *ok = false;

            auto datetime = value.toDateTime();
            if(!datetime.isValid())
                return 0;

            if(ok != nullptr) *ok = true;
            return datetime.toMSecsSinceEpoch();
        }
        default:
            return value.toReal(ok);
    }
}

bool XYSeriesListMapper::isCompleted() const
{
    return completed_;
}

int XYSeriesListMapper::ySectionRole() const
{
    return ySectionRole_;
}

void XYSeriesListMapper::setYSectionRole(int ySectionRole)
{
    if(ySectionRole_ != ySectionRole) {
        ySectionRole_ = ySectionRole;
        emit ySectionRoleChanged();

        if(isCompleted()) {
            for(int i = 0; i < seriesList_.size(); ++i)
                initializeSeriesNameFromModel(i);
        }
    }
}

int XYSeriesListMapper::xSectionRole() const
{
    return xSectionRole_;
}

void XYSeriesListMapper::setXSectionRole(int xSectionRole)
{
    if(xSectionRole_ != xSectionRole) {
        xSectionRole_ = xSectionRole;
        emit xSectionRoleChanged();

        if(isCompleted()) {
            for(int i = 0; i < seriesList_.size(); ++i)
                initializeSeriesNameFromModel(i);
        }
    }
}

int XYSeriesListMapper::averagingInterval() const
{
    return averagingInterval_;
}

void XYSeriesListMapper::setAveragingInterval(int averagingInterval)
{
    if(averagingInterval_ != averagingInterval && averagingInterval != 0) {
        qDebug() << "ave" << averagingInterval;
        averagingInterval_ = averagingInterval;
        emit averagingIntervalChanged();
        for(int i = 0; i < seriesList_.size(); ++i)
            initializeSeriesFromModel(i);
    }
}

int XYSeriesListMapper::seriesNameSection() const
{
    return seriesNameSection_;
}

void XYSeriesListMapper::setSeriesNameSection(int seriesNameSection)
{
    if(seriesNameSection_ != seriesNameSection && (seriesNameSection == 0 || seriesNameSection == 1)) {
        seriesNameSection_ = seriesNameSection;
        emit seriesNameSectionChanged();

        if(isCompleted()) {
            for(int i = 0; i < seriesList_.size(); ++i)
                initializeSeriesNameFromModel(i);
        }
    }
}

int XYSeriesListMapper::numberOfLineInModel() /*const*/
{
    if(model_ == nullptr) return 0;
    int columnCount = model_->columnCount();

    int count = 0;
    for(int i = 0; i < columnCount; ++i) {    // Защита. Ну уж точно не мб быть больше чем columnCount..
        int sectionX = getXSection(i);
        int sectionY = getYSection(i);

        if(sectionX < 0 || sectionY < 0) break;
        if(sectionX >= columnCount || sectionY >= columnCount)
            break;
        ++count;
    }
    return count;
}

bool XYSeriesListMapper::seriesSignalBlocked() const
{
    return seriesSignalBlocked_;
}

void XYSeriesListMapper::setSeriesSignalBlocked(bool seriesSignalBlocked)
{
    seriesSignalBlocked_ = seriesSignalBlocked;
}

QJSValue XYSeriesListMapper::ySectionProvider() const
{
    return ySectionProvider_;
}

void XYSeriesListMapper::setYSectionProvider(const QJSValue &ySectionProvider)
{
    ySectionProvider_ = ySectionProvider;
    emit ySectionProviderChanged();
    //@N Обновить..
    if(isCompleted()) emit updateSeriesList();    // Нужно заново все расчитать..
}

int XYSeriesListMapper::getXSection(int index) /*const*/
{
    if(xSectionProvider_.isNull() || !xSectionProvider_.isCallable())
        return index * 2;
    return xSectionProvider_.call({ QJSValue(index) }).toInt();
}

int XYSeriesListMapper::getYSection(int index) /*const*/
{
    if(ySectionProvider_.isNull() || !ySectionProvider_.isCallable())
        return index * 2 + 1;
    return xSectionProvider_.call({ QJSValue(index) }).toInt();
}

QJSValue XYSeriesListMapper::xSectionProvider() const
{
    return xSectionProvider_;
}

void XYSeriesListMapper::setXSectionProvider(const QJSValue &xSectionProvider)
{
    xSectionProvider_ = xSectionProvider;
    emit xSectionProviderChanged();
    //@N Обновить..
    if(isCompleted()) emit updateSeriesList();    // Нужно заново все расчитать..
}

void XYSeriesListMapper::modelDestroyed()
{
    model_ = nullptr;
    emit reset();
}

void XYSeriesListMapper::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if(!roles.contains(Qt::DisplayRole))    //@N role
        return;
    if(!bottomRight.isValid() || !topLeft.isValid())
        return;

    //@N Мб не всегда имеет смысл обновлять все?

    bool started = false;
    for(int i = 0; i < model_->columnCount(); ++i) {    // Защита. Ну уж точно не мб быть больше чем columnCount..
        int sectionX = getXSection(i);
        int sectionY = getYSection(i);

        if(sectionX < 0 || sectionY < 0) break;
        if((sectionX >= topLeft.column() && sectionX <= bottomRight.column()) ||
                (sectionY >= topLeft.column() && sectionY <= bottomRight.column())) {
            initializeSeriesFromModel(i);
            started = true;
        }
        else if(started) break;
    }
}

void XYSeriesListMapper::rowsInserted(const QModelIndex &parent, int start, int end)
{
    //@N переделать.. Зачем если можно просто добавить новые данные для тех кому нужно..
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)

    for(int i = 0; i < seriesList_.size(); ++i) //@N Мб не всегда имеет смысл обновлять все?
        initializeSeriesFromModel(i);
}

void XYSeriesListMapper::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)

    for(int i = 0; i < seriesList_.size(); ++i) //@N Мб не всегда имеет смысл обновлять все?
        initializeSeriesFromModel(i);
}

void XYSeriesListMapper::columnsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    int startIndex = -1;
    int count = 0;

    for(int i = 0; i < model_->columnCount(); ++i) {    // Защита. Ну уж точно не мб быть больше чем columnCount..
        int sectionX = getXSection(i);
        int sectionY = getYSection(i);

        if(sectionX < 0 || sectionY < 0) break;
        if((sectionX >= start && sectionX <= end) ||
                (sectionY >= start && sectionY <= end)) {
            if(startIndex == -1) startIndex = i;
            ++count;
        }
        else if(startIndex != -1) break;
    }

    emit insertSeries(startIndex, count);
}

void XYSeriesListMapper::columnsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    int startIndex = -1;
    int count = 0;

    for(int i = 0; i <  model_->columnCount() + end; ++i) {    // Защита. Ну уж точно не мб быть больше чем columnCount + end..
        int sectionX = getXSection(i);
        int sectionY = getYSection(i);

        if(sectionX < 0 || sectionY < 0) break;
        if((sectionX >= start && sectionX <= end) ||
                (sectionY >= start && sectionY <= end)) {
            if(startIndex == -1) startIndex = i;
            ++count;
        }
        else if(startIndex != -1) break;
    }

    emit removeSeries(startIndex, count);
}

void XYSeriesListMapper::updateDomain()
{
    if(domainList_.isEmpty()) domain_->reset();
    else if(domainList_.size() == 1) {
        // Один элемент. Значит нужно установить его размеры
        if(seriesList_.first()->isVisible()) {
            auto &d = domainList_.first();
            domain_->setRange(d->minX(), d->maxX(), d->minY(), d->maxY());
        }
        else
            domain_->reset();
    }
    else {
        int count = 0;
        qreal maxX, minX, maxY, minY;
        for(int i = 0; i < domainList_.size(); ++i) {

            if(!seriesList_.at(i)->isVisible()) continue;
            if(static_cast<QXYSeries *>(seriesList_.at(i))->count() == 0) continue;

            const auto &d = domainList_.at(i);
            if(count == 0) {
                minX = d->minX(); maxX = d->maxX();
                minY = d->minY(); maxY = d->maxY();
            }
            else {
                maxX = std::max(d->maxX(), maxX);
                minX = std::min(d->minX(), minX);
                maxY = std::max(d->maxY(), maxY);
                minY = std::min(d->minY(), minY);
            }

            ++count;
        }

        if(count == 0)
            domain_->reset();
        else
            domain_->setRange(minX, maxX, minY, maxY);
    }
}

QT_CHARTS_END_NAMESPACE
