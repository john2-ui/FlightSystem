#ifndef CITYDAO_H
#define CITYDAO_H

#include "../model/City.h"
#include <QList>

class CityDao {
public:
    virtual ~CityDao() = default;
    virtual bool insert(const City& city) = 0;
    virtual bool update(const City& city) = 0;
    virtual bool remove(int id) = 0;
    virtual City getById(int id) = 0;
    virtual QList<City> getAll() = 0;
};

#endif // CITYDAO_H
