QT += core
CONFIG += c++17 staticlib
TEMPLATE = lib

TARGET = model
DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD

HEADERS += Airplane.h \
    Airport.h \
    City.h \
    Flight.h \
    Ticket.h
