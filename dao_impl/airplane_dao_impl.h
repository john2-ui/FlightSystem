#ifndef AIRPLANE_DAO_IMPL_H
#define AIRPLANE_DAO_IMPL_H

#include "../dao/AirplaneDao.h"
#include <QSqlDatabase>

class AirplaneDaoImpl : public AirplaneDao {
public:
    AirplaneDaoImpl();
    int insert(const Airplane& airplane) override;
    bool update(const Airplane& airplane) override;
    bool remove(int id) override;
    Airplane getById(int id) override;
    QList<Airplane> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // AIRPLANE_DAO_IMPL_H
