QT += quick printsupport

CONFIG += c++11

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/src/axis/caxis.h \
    $$PWD/src/axis/cdatetimeaxis.h \
    $$PWD/src/axis/clogvalueaxis.h \
    $$PWD/src/axis/cvalueaxis.h \
    $$PWD/src/cchartview.h \
    $$PWD/src/domain/cdomain.h \
    $$PWD/src/domain/cxydomain.h \
    $$PWD/src/handlers/axisdraghandler.h \
    $$PWD/src/series/clineseries.h \
    $$PWD/src/series/cseries.h \
    $$PWD/src/series/linenode.h

SOURCES += \
    $$PWD/src/axis/caxis.cpp \
    $$PWD/src/axis/cdatetimeaxis.cpp \
    $$PWD/src/axis/clogvalueaxis.cpp \
    $$PWD/src/axis/cvalueaxis.cpp \
    $$PWD/src/cchartview.cpp \
    $$PWD/src/domain/cdomain.cpp \
    $$PWD/src/domain/cxydomain.cpp \
    $$PWD/src/handlers/axisdraghandler.cpp \
    $$PWD/src/series/clineseries.cpp \
    $$PWD/src/series/cseries.cpp \
    $$PWD/src/series/linenode.cpp

RESOURCES += \
    $$PWD/chartview.qrc

TRANSLATIONS = $$PWD/ts/chartview_ru.ts
#lupdate_only {
#    SOURCES += $$PWD/*.qml \
#}

