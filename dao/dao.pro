QT += core sql
CONFIG += c++17
TEMPLATE = lib

TARGET = dao

DESTDIR = $$PWD/../build/lib

INCLUDEPATH += $$PWD    \
        $$PWD/../model

HEADERS += \
    AirplaneDao.h \
    AirportDao.h \
    CityDao.h \
    FlightDao.h \
    TicketDao.h
