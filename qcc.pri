QT += quick

CONFIG += c++11

INCLUDEPATH += $$PWD

CONFIG += using_widgets

CONFIG += dataview          # нужно для того, чтобы не добавлять модуль DataView
CONFIG += chartviewextension

using_widgets {
QT += printsupport
DEFINES += USING_WIDGETS
}

dataview {
DEFINES += DATAVIEW_MODULE           # нужно для того, чтобы не добавлять модуль DataView

TRANSLATIONS = $$PWD/src/QCC/ts/qcc_ru.ts

HEADERS += \
    $$PWD/CChartView \
    $$PWD/src/QCC/DataView/multipledatasource.h

SOURCES += \
    $$PWD/src/QCC/DataView/multipledatasource.cpp

include("$$PWD/src/QCC/DataView/TableView/TableView.pri")
include("$$PWD/src/QCC/DataView/ChartView/ChartView.pri")
}

chartviewextension {
DEFINES += CHARTVIEW_EXTENSION_MODULE
include("$$PWD/src/QCC/DataView/ChartViewExtension/ChartViewExtension.pri")
}


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/src

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

HEADERS +=  \
    $$PWD/src/QCC/Controls.2/vchicon.h \
    $$PWD/src/QCC/Controls/Line/arrowitem.h \
    $$PWD/src/QCC/Controls/Line/line.h \
    $$PWD/src/QCC/Controls/Private/suggestionslist.h \
    $$PWD/src/QCC/Controls.2/waitingbar.h \
    $$PWD/src/QCC/Controls/tooltiplistmodel.h \
    $$PWD/src/QCC/Validators/datevalidator.h \
    $$PWD/qcc.h \
    $$PWD/src/QCC/Validators/hexvalidator.h \
    $$PWD/src/QCC/qccutils.h

SOURCES += \
    $$PWD/qcc.cpp \
    $$PWD/src/QCC/Controls.2/vchicon.cpp \
    $$PWD/src/QCC/Controls/Line/arrowitem.cpp \
    $$PWD/src/QCC/Controls/Line/line.cpp \
    $$PWD/src/QCC/Controls/Private/suggestionslist.cpp \
    $$PWD/src/QCC/Controls.2/waitingbar.cpp \
    $$PWD/src/QCC/Controls/tooltiplistmodel.cpp \
    $$PWD/src/QCC/qccutils.cpp


RESOURCES += \
    $$PWD/qcc.qrc


include("$$PWD/hc.pri")








