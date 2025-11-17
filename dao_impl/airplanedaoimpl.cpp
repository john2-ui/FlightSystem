#include "AirplaneDaoImpl.h"
#include "../db/dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AirplaneDaoImpl::AirplaneDaoImpl() {
    m_db = DBManager::instance().db();
}

bool AirplaneDaoImpl::insert(const Airplane& airplane) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO airplane(model, seats_economy, seats_business, seats_first) "
                  "VALUES(:model, :seats_economy, :seats_business, :seats_first)");
    query.bindValue(":model", airplane.model());
    query.bindValue(":seats_economy", airplane.seatsEconomy());
    query.bindValue(":seats_business", airplane.seatsBusiness());
    query.bindValue(":seats_first", airplane.seatsFirst());
    if (!query.exec()) {
        qDebug() << "Insert airplane failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AirplaneDaoImpl::update(const Airplane& airplane) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE airplane SET model=:model, seats_economy=:seats_economy, "
                  "seats_business=:seats_business, seats_first=:seats_first WHERE id=:id");
    query.bindValue(":model", airplane.model());
    query.bindValue(":seats_economy", airplane.seatsEconomy());
    query.bindValue(":seats_business", airplane.seatsBusiness());
    query.bindValue(":seats_first", airplane.seatsFirst());
    query.bindValue(":id", airplane.id());
    if (!query.exec()) {
        qDebug() << "Update airplane failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AirplaneDaoImpl::remove(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM airplane WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete airplane failed:" << query.lastError().text();
        return false;
    }
    return true;
}

Airplane AirplaneDaoImpl::getById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, model, seats_economy, seats_business, seats_first FROM airplane WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Query airplane failed:" << query.lastError().text();
        return Airplane();
    }
    if (query.next()) {
        return Airplane(query.value("id").toInt(),
                        query.value("model").toString(),
                        query.value("seats_economy").toInt(),
                        query.value("seats_business").toInt(),
                        query.value("seats_first").toInt());
    }
    return Airplane();
}

QList<Airplane> AirplaneDaoImpl::getAll() {
    QList<Airplane> list;
    QSqlQuery query(m_db);
    if (!query.exec("SELECT id, model, seats_economy, seats_business, seats_first FROM airplane")) {
        qDebug() << "Query all airplanes failed:" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        list.append(Airplane(query.value("id").toInt(),
                             query.value("model").toString(),
                             query.value("seats_economy").toInt(),
                             query.value("seats_business").toInt(),
                             query.value("seats_first").toInt()));
    }
    return list;
}
