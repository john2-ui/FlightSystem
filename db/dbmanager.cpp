#include "dbmanager.h"

QSqlDatabase DBManager::db;

bool DBManager::init() {
    // 暂时不连接数据库
    return true;
}
