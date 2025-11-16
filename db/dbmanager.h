#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DBManager {
public:
    static bool init();
    static QSqlDatabase db;
};

#endif // DBMANAGER_H
