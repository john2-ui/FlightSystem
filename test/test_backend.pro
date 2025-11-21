QT += core sql
CONFIG += console c++17
CONFIG -= app_bundle
TEMPLATE = app

# 选择要运行的测试程序
# SOURCES += test_db.cpp          # DAO层测试
SOURCES += test_backend.cpp       # Backend层测试

INCLUDEPATH += ../model \
               ../dao \
               ../dao_impl \
               ../backend \
               ../db

LIBS += -L$$PWD/../build/lib -lbackend -lui_admin -lui_client  -lmodel -ldao_impl -ldb -ldao

