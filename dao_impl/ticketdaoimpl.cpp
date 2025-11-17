#include "TicketDaoImpl.h"
#include "../db/dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

TicketDaoImpl::TicketDaoImpl() {
    m_db = DBManager::instance().db();
}

bool TicketDaoImpl::insert(const Ticket& ticket) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO ticket(flight_id, class, price, total_seats, remain_seats) "
                  "VALUES(:flight_id, :class, :price, :total_seats, :remain_seats)");
    query.bindValue(":flight_id", ticket.flightId());
    query.bindValue(":class", ticket.tClass());
    query.bindValue(":price", ticket.price());
    query.bindValue(":total_seats", ticket.totalSeats());
    query.bindValue(":remain_seats", ticket.remainSeats());
    return query.exec();
}

bool TicketDaoImpl::update(const Ticket& ticket) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE ticket SET flight_id=:flight_id, class=:class, price=:price, total_seats=:total_seats, remain_seats=:remain_seats WHERE id=:id");
    query.bindValue(":flight_id", ticket.flightId());
    query.bindValue(":class", ticket.tClass());
    query.bindValue(":price", ticket.price());
    query.bindValue(":total_seats", ticket.totalSeats());
    query.bindValue(":remain_seats", ticket.remainSeats());
    query.bindValue(":id", ticket.id());
    return query.exec();
}

bool TicketDaoImpl::remove(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM ticket WHERE id=:id");
    query.bindValue(":id", id);
    return query.exec();
}

Ticket TicketDaoImpl::getById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, flight_id, class, price, total_seats, remain_seats FROM ticket WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
    if (query.next()) {
        return Ticket(query.value("id").toInt(),
                      query.value("flight_id").toInt(),
                      query.value("class").toString(),
                      query.value("price").toDouble(),
                      query.value("total_seats").toInt(),
                      query.value("remain_seats").toInt());
    }
    return Ticket();
}

QList<Ticket> TicketDaoImpl::getAll() {
    QList<Ticket> list;
    QSqlQuery query(m_db);
    query.exec("SELECT id, flight_id, class, price, total_seats, remain_seats FROM ticket");
    while (query.next()) {
        list.append(Ticket(query.value("id").toInt(),
                           query.value("flight_id").toInt(),
                           query.value("class").toString(),
                           query.value("price").toDouble(),
                           query.value("total_seats").toInt(),
                           query.value("remain_seats").toInt()));
    }
    return list;
}
