QT += core gui widgets sql

CONFIG += c++17

TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

LIBS += -L$$PWD/../db -ldb
INCLUDEPATH += $$PWD/../db
