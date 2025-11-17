#ifndef AIRPLANEDAOIMPL_H
#define AIRPLANEDAOIMPL_H

#include "../dao/AirplaneDao.h"
#include <QSqlDatabase>

class AirplaneDaoImpl : public AirplaneDao {
public:
    AirplaneDaoImpl();
    bool insert(const Airplane& airplane) override;
    bool update(const Airplane& airplane) override;
    bool remove(int id) override;
    Airplane getById(int id) override;
    QList<Airplane> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // AIRPLANEDAOIMPL_H
