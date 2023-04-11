#include "qcc.h"

#include "src/QCC/Controls/Line/line.h"
#include "src/QCC/Controls/Line/arrowitem.h"
#include "src/QCC/Validators/datevalidator.h"
#include "src/QCC/Validators/hexvalidator.h"

#include "src/QCC/Controls/Private/suggestionslist.h"

#include "src/QCC/Controls.2/waitingbar.h"
#include "src/QCC/Controls.2/vchicon.h"
#include "src/QCC/Controls/tooltiplistmodel.h"

#include <QDebug>
#include <QTimer>

#include <SortFilterProxyModel>
#include "src/helperclasses/filters/sortfilterproxytablemodelv1.h"

#include <CTranslator>

#include "src/QCC/qccutils.h"

#ifdef DATAVIEW_MODULE
#include "CChartView"
#include "src/QCC/DataView/TableView/selectionmodel.h"
#endif

#ifdef CHARTVIEW_EXTENSION_MODULE
#include "chartviewextension.h"
#include "xyserieslistmapper.h"
#endif


class QCCRegister {
public:
    QCCRegister() {}

    void registerTypes(QQmlApplicationEngine *engine) {
        this->engine = engine;
        setup();


        //        if(engine == nullptr)
        //            return;

        //        if(this->engine != engine) {
        //            this->engine = engine;
        //            setup();
        //        }
        //        else if(!engine->importPathList().contains(importPath)) {   //@N А что делать если engine при создании такой же адрес имеет..
        //            setup();
        //        }
    }

private:
    QQmlApplicationEngine *engine = nullptr;
    const QString importPath = "qrc:/src";

    void setup() {
        addTranslations(); //@N нужно ли тут?

        //        qmlRegisterType <QTimer> ("QCC", 1, 0, "Timer");
        qmlRegisterType <SortFilterProxyListModel> ("QCC.Filters", 1, 0, "SortFilterProxyListModel");
        qmlRegisterType <SortFilterProxyTableModelV1> ("QCC.Filters", 1, 0, "SortFilterProxyTableModel");


        qmlRegisterType <SortFilterProxyTableModel> ("QCC.Filters", 2, 0, "SortFilterProxyTableModel");
        qmlRegisterType <TableFilterListModel> ("QCC.Filters", 2, 0, "TableFilterListModel");
        qRegisterMetaType <TableFilterListModel::TypeCondition> ("TableFilterListModel::TypeCondition");


        // Controls
        qmlRegisterType <Line> ("QCC.Controls", 1, 0, "Line");

        qmlRegisterType <ArrowItem> ("QCC.Controls", 1, 0, "ArrowItem");
        qRegisterMetaType <ArrowItem::IndicatorForm> ("ArrowItem::IndicatorForm");



        qmlRegisterType <WaitingBar> ("QCC.Controls", 2, 0, "WaitingBar");
        qmlRegisterType <VchIcon> ("QCC.Controls", 2, 0, "VchIcon");
        qmlRegisterType <SuggestionsList> ("QCC.Controls.Private", 1, 0, "SuggestionsList");
        qmlRegisterType <ToolTipListModel> ("QCC.Controls.Private", 1, 0, "ToolTipListModel");
        qRegisterMetaType <ToolTipListModel::ListType> ("ToolTipListModel::ListType");


        // Validators
        qmlRegisterType <DateValidator> ("QCC.Validators", 1, 0, "DateValidator");
        qmlRegisterType <HexValidator> ("QCC.Validators", 1, 0, "HexValidator");


        qmlRegisterSingletonType<QCCUtils>("QCC.Utils", 1, 0, "Utils", &QCCUtils::qmlInstance);


#ifdef DATAVIEW_MODULE
        // TableView
        qmlRegisterType <SelectionModel> ("QCC.DataView.TableView", 1, 0, "SelectionModel");

        // ChartView
        qmlRegisterType <CSeries> ("QCC.DataView.ChartView", 1, 0, "Series");

        qmlRegisterType <CChartView> ("QCC.DataView.ChartView", 1, 0, "ChartView");
        qmlRegisterType <CLineSeries> ("QCC.DataView.ChartView", 1, 0, "LineSeries");

        qmlRegisterType <CLogValueAxis>("QCC.DataView.ChartView", 1, 0, "LogValueAxis");
        qmlRegisterType <CValueAxis>("QCC.DataView.ChartView", 1, 0, "ValueAxis");
        qmlRegisterType <CDateTimeAxis>("QCC.DataView.ChartView", 1, 0, "DateTimeAxis");

        qmlRegisterType <AxisDragHandler>("QCC.DataView.ChartView", 1, 0, "AxisDragHandler");
        qRegisterMetaType <QList<CSeries*>> ("QList<CSeries*>");
#endif

#ifdef CHARTVIEW_EXTENSION_MODULE
        qmlRegisterUncreatableMetaObject(QtCharts::ChartViewExtension::staticMetaObject, "QCC.DataView.ChartViewExtension", 1, 0, "ChartViewExtension", "" );
        qmlRegisterType <QtCharts::XYSeriesListMapper>("QCC.DataView.ChartViewExtension", 1, 0, "XYSeriesListMapper");
        qRegisterMetaType <XYDomain*> ("XYDomain*");
#endif

        if(engine != nullptr) {
            engine->addImportPath(importPath);
        }
    }

    void addTranslations()
    {
        CTranslator::getInstance()->appendLanguage(QLocale::Russian, ":/ts/qcc_ru.qm");

#ifdef DATAVIEW_MODULE
        CTranslator::getInstance()->appendLanguage(QLocale::Russian, ":/ts/chartview_ru.qm");
        CTranslator::getInstance()->appendLanguage(QLocale::Russian, ":/ts/tableview_ru.qm");
#endif

#ifdef CHARTVIEW_EXTENSION_MODULE
        CTranslator::getInstance()->appendLanguage(QLocale::Russian, ":/ts/chartviewextension_ru.qm");
#endif
    }
};

void QCC::registerTypes(QQmlApplicationEngine *engine)
{
    static QCCRegister qccReg;
    qccReg.registerTypes(engine);
}





