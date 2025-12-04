#include "user_dao_impl.h"
#include "../db/dbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

UserDaoImpl::UserDaoImpl() {
    m_db = DBManager::instance().db();
}

int UserDaoImpl::insert(const User& user) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO user (username, password, tickets_id, isSuper) "
                  "VALUES (:username, :password, :tickets_id, :isSuper)");
    query.bindValue(":username", user.username());
    query.bindValue(":password", user.password()); 
    //当空票单的时候也能注册
    const QString ticketStr = user.ticketsIDToString();
    query.bindValue(":tickets_id", ticketStr.isEmpty() ? QStringLiteral("") : ticketStr);
    query.bindValue(":isSuper", user.isSuper());   

    if (!query.exec()) {
        qDebug() << "Insert user failed:" << query.lastError();
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool UserDaoImpl::update(const User& user) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE user SET username=:username, password=:password, "
                  "tickets_id=:tickets_id, isSuper=:isSuper WHERE id=:id");

    query.bindValue(":username", user.username());
    query.bindValue(":password", user.password()); 
    //当空票单的时候也能注册
    const QString ticketStr = user.ticketsIDToString();
    query.bindValue(":tickets_id", ticketStr.isEmpty() ? QStringLiteral("") : ticketStr); 
    query.bindValue(":isSuper", user.isSuper());   
    query.bindValue(":id", user.id());

    if (!query.exec()) {
        qDebug() << "Update user failed:" << query.lastError();
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool UserDaoImpl::remove(int id) {
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM user WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Delete user failed:" << query.lastError();
        return false;
    }

    return query.numRowsAffected() > 0;
}

User UserDaoImpl::getById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, username, password, tickets_id, isSuper "
                  "FROM user WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        return User(
            query.value(0).toInt(),                         // id
            query.value(1).toString(),                      // username
            query.value(2).toString(),                      // password
            User::parseTicketsID(query.value(3).toString()),// tickets
            query.value(4).toInt()                          // isSuper
        );
    }

    return User();  // empty
}

User UserDaoImpl::getByUsername(const QString& username) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, username, password, tickets_id, isSuper "
                  "FROM user WHERE username=:username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        return User(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            User::parseTicketsID(query.value(3).toString()),
            query.value(4).toInt()   // isSuper
        );
    }

    return User();
}

QList<User> UserDaoImpl::getAll() {
    QList<User> users;
    QSqlQuery query(m_db);
    query.exec("SELECT id, username, password, tickets_id, isSuper FROM user");

    while (query.next()) {
        users.append(User(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            User::parseTicketsID(query.value(3).toString()),
            query.value(4).toInt()   //  isSuper
        ));
    }

    return users;
}
