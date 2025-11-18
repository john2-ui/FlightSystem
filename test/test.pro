QT += core gui widgets sql
CONFIG += c++17
TEMPLATE = app

SOURCES += test_db.cpp

INCLUDEPATH += $$PWD/../db

LIBS += -L$$PWD/../build/lib -ldb -lbackend -lmodel -lui_admin -lui_client -ldao
