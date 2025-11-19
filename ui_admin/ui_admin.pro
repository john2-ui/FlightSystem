QT += core sql
CONFIG += c++17 staticlib
TEMPLATE = lib

# 跨平台共享库名（Windows生成 db.lib，mac生成 libdb.a）
TARGET = ui_admin

# ⭐ 所有平台统一输出到 bin 目录（windows + macos）
DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD

SOURCES += dummy.cpp
HEADERS += dummy.h
