QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

CONFIG += staticlib

TARGET = ui_admin
DESTDIR = $$PWD/../build/lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/../backend \
    $$PWD/../model \
    $$PWD/../dao

SOURCES += \
    ui_admin.cpp

HEADERS += \
    ui_admin.h \
    ui_ui_admin.h

FORMS += \
    ui_admin.ui


LIBS += -L../build/lib -lbackend -ldb -lmodel -ldao_impl
