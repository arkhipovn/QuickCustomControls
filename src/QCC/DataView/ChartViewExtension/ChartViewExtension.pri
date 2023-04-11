QT += quick charts

CONFIG += c++11

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/chartviewextension.h \
    $$PWD/xydomain.h \
    $$PWD/xyserieslistmapper.h


SOURCES += \
    $$PWD/xydomain.cpp \
    $$PWD/xyserieslistmapper.cpp


RESOURCES += \
    $$PWD/chartviewextension.qrc


TRANSLATIONS = $$PWD/ts/chartviewextension_ru.ts
