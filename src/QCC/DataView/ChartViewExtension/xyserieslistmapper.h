#ifndef XYSERIESLISTMAPPER_H
#define XYSERIESLISTMAPPER_H

#include <QObject>
#include <QJSValue>
#include <QQmlParserStatus>
#include <QXYSeries>
#include <QAbstractItemModel>
#include "xydomain.h"

QT_CHARTS_BEGIN_NAMESPACE

class XYSeriesListMapper : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)

    Q_PROPERTY(QJSValue xSectionProvider READ xSectionProvider WRITE setXSectionProvider NOTIFY xSectionProviderChanged)
    Q_PROPERTY(QJSValue ySectionProvider READ ySectionProvider WRITE setYSectionProvider NOTIFY ySectionProviderChanged)

    Q_PROPERTY(int seriesNameSection READ seriesNameSection WRITE setSeriesNameSection NOTIFY seriesNameSectionChanged)
    Q_PROPERTY(int averagingInterval READ averagingInterval WRITE setAveragingInterval NOTIFY averagingIntervalChanged)

    Q_PROPERTY(int xSectionRole READ xSectionRole WRITE setXSectionRole NOTIFY xSectionRoleChanged)
    Q_PROPERTY(int ySectionRole READ ySectionRole WRITE setYSectionRole NOTIFY ySectionRoleChanged)

public:
    explicit XYSeriesListMapper(QObject *parent = nullptr);
    virtual ~XYSeriesListMapper();

    void classBegin() override;
    void componentComplete() override;

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    QJSValue xSectionProvider() const;
    void setXSectionProvider(const QJSValue &xSectionProvider);

    QJSValue ySectionProvider() const;
    void setYSectionProvider(const QJSValue &ySectionProvider);

    int getXSection(int index) /*const*/;   // QJSValue::call ### Qt6: Make const
    int getYSection(int index) /*const*/;   // QJSValue::call ### Qt6: Make const

    bool seriesSignalBlocked() const;
    void setSeriesSignalBlocked(bool seriesSignalBlocked);

    int seriesNameSection() const;
    void setSeriesNameSection(int seriesNameSection);

    int averagingInterval() const;
    void setAveragingInterval(int averagingInterval);

    int xSectionRole() const;
    void setXSectionRole(int xSectionRole);

    int ySectionRole() const;
    void setYSectionRole(int ySectionRole);

public:
    Q_INVOKABLE void append(QAbstractSeries *series);
    Q_INVOKABLE void insert(int index, QAbstractSeries *series);
    Q_INVOKABLE void removeAt(int index);
    Q_INVOKABLE void removeAll();
    Q_INVOKABLE XYDomain* domainAt(int index) const;
    Q_INVOKABLE XYDomain* domain() const;  // мин/макс среди всех линий

    // Возвращает количество возможных LineSeries с учетом xSectionProvider_ и ySectionProvider_
    Q_INVOKABLE int numberOfLineInModel() /*const*/;    // QJSValue::call ### Qt6: Make const

public slots:
    // изменение размеров
    void updateDomain();

protected slots:
    void initializeSeriesNameFromModel(int index);
    void initializeSeriesFromModel(int index);
    void modelDestroyed();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent, int start, int end);
    void columnsInserted(const QModelIndex &parent, int start, int end);
    void columnsRemoved(const QModelIndex &parent, int start, int end);
    void headerDataChanged(Qt::Orientation orientation, int start, int end);

signals:
    void updateSeriesList();                                // Нужно удалить все предыдущие и создает новые линии
    void reset();                                           // Нужно очистить все линии
    void insertSeries(int index, int count);                // Нужно добавить 1 или несколько линий
    void removeSeries(int index, int count);                // Нужно удалить 1 или несколкьо линий


    void xSectionProviderChanged();
    void ySectionProviderChanged();
    void modelChanged();
    void seriesNameSectionChanged();
    void averagingIntervalChanged();

    void xSectionRoleChanged();
    void ySectionRoleChanged();

protected:
    qreal valueFromModel(int row, int column, int role, bool *ok = nullptr) const;
    qreal valueFromModel(const QModelIndex &index, int role, bool *ok = nullptr) const;
    bool isCompleted() const;

protected:
    XYDomain *domain_;                          // мин/макс среди всех линий

    QAbstractItemModel *model_;
    QList <QAbstractSeries*> seriesList_;       // список линий из chartview
    QList <XYDomain*> domainList_;              // мин/макс для каждой линии

    // функции для получания индексов
    QJSValue xSectionProvider_;
    QJSValue ySectionProvider_;

    int xSectionRole_;
    int ySectionRole_;

    int seriesNameSection_;                      // 0 или 1. Какую колонку использовать

    int averagingInterval_;                      // усреднение по горизонтальной оси

    bool completed_;
    bool seriesSignalBlocked_;  //@N пока что не используется, но в будущем добавить
};

QT_CHARTS_END_NAMESPACE

#endif // XYSERIESLISTMAPPER_H
