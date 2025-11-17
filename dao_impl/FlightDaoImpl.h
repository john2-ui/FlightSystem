#ifndef FLIGHTDAOIMPL_H
#define FLIGHTDAOIMPL_H

#include "../dao/FlightDao.h"
#include <QSqlDatabase>

class FlightDaoImpl : public FlightDao {
public:
    FlightDaoImpl();
    bool insert(const Flight& flight) override;
    bool update(const Flight& flight) override;
    bool remove(int id) override;
    Flight getById(int id) override;
    QList<Flight> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // FLIGHTDAOIMPL_H
