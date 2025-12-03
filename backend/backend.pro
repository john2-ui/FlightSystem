QT += core sql
CONFIG += c++17 staticlib
TEMPLATE = lib

TARGET = backend
DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD \
               $$PWD/../dao \
               $$PWD/../dao_impl \
               $$PWD/../model \
               $$PWD/../db

SOURCES += backend.cpp
HEADERS += backend.h

# backend 依赖 dao_impl 和 db
LIBS += -L$$PWD/../build/lib -ldao_impl -ldb -lmodel
