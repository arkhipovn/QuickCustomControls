QT       += core

CONFIG += c++11
INCLUDEPATH += $$PWD

contains(QT, sql) {
SOURCES += \
    $$PWD/src/helperclasses/models/sqltablemodel.cpp \

HEADERS += \
    $$PWD/src/helperclasses/models/sqltablemodel.h \
}

SOURCES += \
    $$PWD/src/helperclasses/cringbuffer.cpp \
    $$PWD/src/helperclasses/csortedlist.cpp \
    $$PWD/src/helperclasses/ctranslator.cpp \
    $$PWD/src/helperclasses/filters/sortfilterproxylistmodel.cpp \
    $$PWD/src/helperclasses/filters/sortfilterproxytablemodel.cpp \
    $$PWD/src/helperclasses/filters/sortfilterproxytablemodelv1.cpp \
    $$PWD/src/helperclasses/filters/tablefilterlistmodel.cpp

HEADERS += \
    $$PWD/CRingBuffer \
    $$PWD/CSortedList \
    $$PWD/CTranslator \
    $$PWD/SortFilterProxyModel \
    $$PWD/src/helperclasses/cringbuffer.h \
    $$PWD/src/helperclasses/csortedlist.h \
    $$PWD/src/helperclasses/ctranslator.h \
    $$PWD/src/helperclasses/filters/sortfilterproxylistmodel.h \
    $$PWD/src/helperclasses/filters/sortfilterproxytablemodel.h \
    $$PWD/src/helperclasses/filters/sortfilterproxytablemodelv1.h \
    $$PWD/src/helperclasses/filters/tablefilterlistmodel.h
