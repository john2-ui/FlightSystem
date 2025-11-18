#ifndef AIRPORTDAOIMPL_H
#define AIRPORTDAOIMPL_H

#include "../dao/AirportDao.h"
#include <QSqlDatabase>

class AirportDaoImpl : public AirportDao {
public:
    AirportDaoImpl();
    bool insert(const Airport& airport) override;
    bool update(const Airport& airport) override;
    bool remove(int id) override;
    Airport getById(int id) override;
    QList<Airport> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // AIRPORTDAOIMPL_H
