#ifndef CITY_DAO_IMPL_H
#define CITY_DAO_IMPL_H

#include "../dao/CityDao.h"
#include <QSqlDatabase>

class CityDaoImpl : public CityDao {
public:
    CityDaoImpl();
    int insert(const City& city) override;
    bool update(const City& city) override;
    bool remove(int id) override;
    City getById(int id) override;
    QList<City> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // CITY_DAO_IMPL_H
