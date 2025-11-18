QT += core sql
TEMPLATE = lib
CONFIG += staticlib
TARGET = dao_impl

CONFIG += c++17

DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD ../dao ../db ../model

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
