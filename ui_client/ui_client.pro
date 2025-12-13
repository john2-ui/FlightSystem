QT += core gui widgets sql
CONFIG += c++17 staticlib
TEMPLATE = lib

# 跨平台共享库名（Windows生成 db.lib，mac生成 libdb.a）
TARGET = ui_client

# ⭐ 所有平台统一输出到 bin 目录（windows + macos）
DESTDIR = $$PWD/../build/lib

INCLUDEPATH += \
    $$PWD/../app



SOURCES += \
    ui_client.cpp

HEADERS += \
    ui_client.h \
    ui_ui_client.h


FORMS += \
    ui_client.ui


