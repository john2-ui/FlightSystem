#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <QString>

class Airplane {
public:
    Airplane() = default;
    Airplane(int id, const QString& model, int seatsEconomy, int seatsBusiness, int seatsFirst)
        : m_id(id), m_model(model), m_seatsEconomy(seatsEconomy), m_seatsBusiness(seatsBusiness), m_seatsFirst(seatsFirst) {}

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString model() const { return m_model; }
    void setModel(const QString& model) { m_model = model; }

    int seatsEconomy() const { return m_seatsEconomy; }
    void setSeatsEconomy(int seats) { m_seatsEconomy = seats; }

    int seatsBusiness() const { return m_seatsBusiness; }
    void setSeatsBusiness(int seats) { m_seatsBusiness = seats; }

    int seatsFirst() const { return m_seatsFirst; }
    void setSeatsFirst(int seats) { m_seatsFirst = seats; }

private:
    int m_id;
    QString m_model;
    int m_seatsEconomy;
    int m_seatsBusiness;
    int m_seatsFirst;
};

#endif // AIRPLANE_H
