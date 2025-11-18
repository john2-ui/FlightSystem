QT += core sql
CONFIG += console c++17
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += test_db.cpp
INCLUDEPATH += ../model \
               ../dao \
               ../dao_impl

LIBS += -L$$PWD/../build/lib -lbackend -lui_admin -lui_client  -lmodel -ldao_impl -ldb -ldao

