QT += core
CONFIG += c++17 staticlib
TEMPLATE = lib

TARGET = backend
DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD

SOURCES += backend.cpp
HEADERS += backend.h
