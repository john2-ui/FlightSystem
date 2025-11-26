#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QDebug>
#include <QSqlDatabase>

class DBManager {
public:
    static DBManager& instance();
    QSqlDatabase& db();
    void close();
private:
    DBManager();
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
