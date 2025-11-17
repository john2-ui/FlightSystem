#ifndef CITYDAOIMPL_H
#define CITYDAOIMPL_H

#include "../dao/CityDao.h"
#include <QSqlDatabase>

class CityDaoImpl : public CityDao {
public:
    CityDaoImpl();
    bool insert(const City& city) override;
    bool update(const City& city) override;
    bool remove(int id) override;
    City getById(int id) override;
    QList<City> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // CITYDAOIMPL_H
