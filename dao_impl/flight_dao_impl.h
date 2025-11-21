#ifndef FLIGHT_DAO_IMPL_H
#define FLIGHT_DAO_IMPL_H

#include "../dao/FlightDao.h"
#include <QSqlDatabase>

class FlightDaoImpl : public FlightDao {
public:
    FlightDaoImpl();
    int insert(const Flight& flight) override;
    bool update(const Flight& flight) override;
    bool remove(int id) override;
    Flight getById(int id) override;
    QList<Flight> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // FLIGHT_DAO_IMPL_H
