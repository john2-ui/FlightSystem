#ifndef AIRPLANEDAO_H
#define AIRPLANEDAO_H

#include "../model/Airplane.h"
#include <QList>

class AirplaneDao {
public:
    virtual ~AirplaneDao() = default;
    virtual int insert(const Airplane& airplane) = 0;
    virtual bool update(const Airplane& airplane) = 0;
    virtual bool remove(int id) = 0;
    virtual Airplane getById(int id) = 0;
    virtual QList<Airplane> getAll() = 0;
};

#endif
