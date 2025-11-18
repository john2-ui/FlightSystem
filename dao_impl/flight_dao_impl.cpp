#include "FlightDaoImpl.h"
#include "../db/dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

FlightDaoImpl::FlightDaoImpl() {
    m_db = DBManager::instance().db();
}

bool FlightDaoImpl::insert(const Flight& flight) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO flight(flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status) "
                  "VALUES(:flight_no, :airplane_id, :depart_airport_id, :arrive_airport_id, :depart_time, :arrive_time, :status)");
    query.bindValue(":flight_no", flight.flightNo());
    query.bindValue(":airplane_id", flight.airplaneId());
    query.bindValue(":depart_airport_id", flight.departAirportId());
    query.bindValue(":arrive_airport_id", flight.arriveAirportId());
    query.bindValue(":depart_time", flight.departTime());
    query.bindValue(":arrive_time", flight.arriveTime());
    query.bindValue(":status", flight.status());
    if (!query.exec()) {
        qDebug() << "Insert flight failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool FlightDaoImpl::update(const Flight& flight) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE flight SET flight_no=:flight_no, airplane_id=:airplane_id, depart_airport_id=:depart_airport_id, "
                  "arrive_airport_id=:arrive_airport_id, depart_time=:depart_time, arrive_time=:arrive_time, status=:status WHERE id=:id");
    query.bindValue(":flight_no", flight.flightNo());
    query.bindValue(":airplane_id", flight.airplaneId());
    query.bindValue(":depart_airport_id", flight.departAirportId());
    query.bindValue(":arrive_airport_id", flight.arriveAirportId());
    query.bindValue(":depart_time", flight.departTime());
    query.bindValue(":arrive_time", flight.arriveTime());
    query.bindValue(":status", flight.status());
    query.bindValue(":id", flight.id());
    if (!query.exec()) {
        qDebug() << "Update flight failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool FlightDaoImpl::remove(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM flight WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete flight failed:" << query.lastError().text();
        return false;
    }
    return true;
}

Flight FlightDaoImpl::getById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status FROM flight WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Query flight failed:" << query.lastError().text();
        return Flight();
    }
    if (query.next()) {
        return Flight(query.value("id").toInt(),
                      query.value("flight_no").toString(),
                      query.value("airplane_id").toInt(),
                      query.value("depart_airport_id").toInt(),
                      query.value("arrive_airport_id").toInt(),
                      query.value("depart_time").toDateTime(),
                      query.value("arrive_time").toDateTime(),
                      query.value("status").toString());
    }
    return Flight();
}

QList<Flight> FlightDaoImpl::getAll() {
    QList<Flight> list;
    QSqlQuery query(m_db);
    if (!query.exec("SELECT id, flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status FROM flight")) {
        qDebug() << "Query all flights failed:" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        list.append(Flight(query.value("id").toInt(),
                           query.value("flight_no").toString(),
                           query.value("airplane_id").toInt(),
                           query.value("depart_airport_id").toInt(),
                           query.value("arrive_airport_id").toInt(),
                           query.value("depart_time").toDateTime(),
                           query.value("arrive_time").toDateTime(),
                           query.value("status").toString()));
    }
    return list;
}
