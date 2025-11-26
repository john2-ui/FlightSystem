#include "airport_dao_impl.h"
#include "../db/DBManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AirportDaoImpl::AirportDaoImpl() {
    m_db = DBManager::instance().db();
}

int AirportDaoImpl::insert(const Airport& airport) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO airport(name, code, city_id, terminal_count) VALUES(:name, :code, :city_id, :terminal_count)");
    query.bindValue(":name", airport.name());
    query.bindValue(":code", airport.code());
    query.bindValue(":city_id", airport.cityId());
    query.bindValue(":terminal_count", airport.terminalCount());
    if (!query.exec()) {
        qDebug() << "Insert airport failed:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

bool AirportDaoImpl::update(const Airport& airport) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE airport SET name=:name, code=:code, city_id=:city_id, terminal_count=:terminal_count WHERE id=:id");
    query.bindValue(":name", airport.name());
    query.bindValue(":code", airport.code());
    query.bindValue(":city_id", airport.cityId());
    query.bindValue(":terminal_count", airport.terminalCount());
    query.bindValue(":id", airport.id());
    if (!query.exec()) {
        qDebug() << "Update airport failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AirportDaoImpl::remove(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM airport WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete airport failed:" << query.lastError().text();
        return false;
    }
    return true;
}

Airport AirportDaoImpl::getById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, code, city_id, terminal_count FROM airport WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Query airport failed:" << query.lastError().text();
        return Airport();
    }
    if (query.next()) {
        return Airport(query.value("id").toInt(),
                       query.value("name").toString(),
                       query.value("code").toString(),
                       query.value("city_id").toInt(),
                       query.value("terminal_count").toInt());
    }
    return Airport();
}

QList<Airport> AirportDaoImpl::getAll() {
    QList<Airport> list;
    QSqlQuery query(m_db);
    if (!query.exec("SELECT id, name, code, city_id, terminal_count FROM airport")) {
        qDebug() << "Query all airports failed:" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        list.append(Airport(query.value("id").toInt(),
                            query.value("name").toString(),
                            query.value("code").toString(),
                            query.value("city_id").toInt(),
                            query.value("terminal_count").toInt()));
    }
    return list;
}
