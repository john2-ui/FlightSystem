#ifndef AIRPORTDAO_H
#define AIRPORTDAO_H

#include "../model/Airport.h"
#include <QList>

class AirportDao {
public:
    virtual ~AirportDao() = default;
    virtual int insert(const Airport& airport) = 0;
    virtual bool update(const Airport& airport) = 0;
    virtual bool remove(int id) = 0;
    virtual Airport getById(int id) = 0;
    virtual QList<Airport> getAll() = 0;
};

#endif
