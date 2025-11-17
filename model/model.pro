QT += core
CONFIG += c++17 staticlib
TEMPLATE = lib

TARGET = model
DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD

SOURCES += model.cpp
HEADERS += model.h
