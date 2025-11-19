#ifndef CITY_H
#define CITY_H

#include <QString>

class City {
public:
    City() = default;
    City(int id, const QString& name, const QString& code, const QString& country)
        : m_id(id), m_name(name), m_code(code), m_country(country) {}

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString code() const { return m_code; }
    void setCode(const QString& code) { m_code = code; }

    QString country() const { return m_country; }
    void setCountry(const QString& country) { m_country = country; }

private:
    int m_id;
    QString m_name;
    QString m_code;
    QString m_country;
};

#endif // CITY_H
