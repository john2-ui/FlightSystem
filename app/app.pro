QT += core gui widgets sql
CONFIG += c++17
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

INCLUDEPATH += $$PWD/../db

LIBS += -L$$PWD/../build/lib -ldb -lbackend -lmodel -lui_admin -lui_client
