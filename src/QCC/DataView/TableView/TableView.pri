QT += quick printsupport

CONFIG += c++11

INCLUDEPATH += $$PWD


HEADERS += \
    $$PWD/selectionmodel.h

SOURCES += \
    $$PWD/selectionmodel.cpp

RESOURCES += \
    $$PWD/tableview.qrc

TRANSLATIONS = $$PWD/ts/tableview_ru.ts

#lupdate_only {
#    SOURCES += $$PWD/*.qml \
#    SOURCES += $$PWD/Private/*.qml #\
#}
