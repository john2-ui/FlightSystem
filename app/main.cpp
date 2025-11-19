#include "mainwindow.h"

// #include <QApplication>
// #include <QSqlDatabase>
// #include <QDebug>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     qDebug() << "支持数据库：" << QSqlDatabase::drivers();
//     w.show();
//     return a.exec();
// }


#include <QCoreApplication>
#include <QtSql>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 1. 添加 MySQL 数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");      // MySQL 服务器地址
    db.setPort(3306);                 // MySQL 端口
    db.setDatabaseName("flightsystem");     // 数据库名称
    db.setUserName("root");           // 用户名
    db.setPassword("491122");         // 密码



    // 2. 尝试连接数据库
    if (!db.open()) {
        qDebug() << "数据库连接失败:" << db.lastError().text();
        return -1;
    }
    qDebug() << "数据库连接成功!";

    return 0;
}
