QT += core sql
TEMPLATE = lib
CONFIG += staticlib
TARGET = dao_impl

CONFIG += c++17

DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD  ../db ../model ../dao

SOURCES += \
    flight_dao_impl.cpp \
    airplane_dao_impl.cpp \
    airport_dao_impl.cpp \
    city_dao_impl.cpp \
    ticket_dao_impl.cpp

HEADERS += \
    flight_dao_impl.h \
    airplane_dao_impl.h \
    airport_dao_impl.h \
    city_dao_impl.h \
    ticket_dao_impl.h

LIBS += -L$$PWD/../build/lib -ldb
