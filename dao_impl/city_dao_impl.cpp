#include "city_dao_impl.h"
#include "../db/DBManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CityDaoImpl::CityDaoImpl() {
    m_db = DBManager::instance().db();
}

int CityDaoImpl::insert(const City& city) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO city(name, code, country) VALUES(:name, :code, :country)");
    query.bindValue(":name", city.name());
    query.bindValue(":code", city.code());
    query.bindValue(":country", city.country());
    if (!query.exec()) {
        qDebug() << "Insert city failed:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

bool CityDaoImpl::update(const City& city) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE city SET name=:name, code=:code, country=:country WHERE id=:id");
    query.bindValue(":name", city.name());
    query.bindValue(":code", city.code());
    query.bindValue(":country", city.country());
    query.bindValue(":id", city.id());
    if (!query.exec()) {
        qDebug() << "Update city failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CityDaoImpl::remove(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM city WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete city failed:" << query.lastError().text();
        return false;
    }
    return true;
}

City CityDaoImpl::getById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, code, country FROM city WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Query city failed:" << query.lastError().text();
        return City();
    }
    if (query.next()) {
        return City(query.value("id").toInt(),
                    query.value("name").toString(),
                    query.value("code").toString(),
                    query.value("country").toString());
    }
    return City();
}

QList<City> CityDaoImpl::getAll() {
    QList<City> list;
    QSqlQuery query(m_db);
    if (!query.exec("SELECT id, name, code, country FROM city")) {
        qDebug() << "Query all cities failed:" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        list.append(City(query.value("id").toInt(),
                         query.value("name").toString(),
                         query.value("code").toString(),
                         query.value("country").toString()));
    }
    return list;
}
