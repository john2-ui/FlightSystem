#ifndef USER_DAO_IMPL_H
#define USER_DAO_IMPL_H
#include "../dao/UserDao.h"
#include <QSqlDatabase>

class UserDaoImpl : public UserDao{
public:
    UserDaoImpl();
    int insert(const User& user) override;
    bool update(const User& user) override;
    bool remove(int id) override;
    User getById(int id) override;
    User getByUsername(const QString& username) override;
    QList<User> getAll() override;
private:
    QSqlDatabase m_db;
};

#endif
