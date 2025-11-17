#ifndef FLIGHTDAO_H
#define FLIGHTDAO_H

#include "../model/Flight.h"
#include <QList>

class FlightDao {
public:
    virtual ~FlightDao() = default;
    virtual bool insert(const Flight& flight) = 0;
    virtual bool update(const Flight& flight) = 0;
    virtual bool remove(int id) = 0;
    virtual Flight getById(int id) = 0;
    virtual QList<Flight> getAll() = 0;
};

#endif // FLIGHTDAO_H
