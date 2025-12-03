#ifndef USERDAO_H
#define USERDAO_H

#include "../model/User.h"

class UserDao {
public:
    virtual ~UserDao() = default;

    virtual int insert(const User& user) = 0;
    virtual bool update(const User& user) = 0;
    virtual bool remove(int id) = 0;
    virtual User getById(int id) = 0;
    virtual User getByUsername(const QString& username) = 0;
    virtual QList<User> getAll() = 0;
};

#endif // USERDAO_H
