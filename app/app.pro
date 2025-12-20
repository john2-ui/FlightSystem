QT += core gui widgets sql
CONFIG += c++17
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

INCLUDEPATH += \
    $$PWD/../db \
    $$PWD/../ui_admin \
    $$PWD/../ui_client

LIBS += -L$$PWD/../build/lib -lui_client -lui_admin -lbackend -ldb -lmodel  -ldao_impl
