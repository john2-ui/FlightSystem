#ifndef TICKET_H
#define TICKET_H

#include <QString>

class Ticket {
public:
    Ticket() = default;
    Ticket(int id, int flightId, const QString& tclass, double price, int totalSeats, int remainSeats)
        : m_id(id), m_flightId(flightId), m_class(tclass), m_price(price), m_totalSeats(totalSeats), m_remainSeats(remainSeats) {}

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int flightId() const { return m_flightId; }
    void setFlightId(int id) { m_flightId = id; }

    QString tClass() const { return m_class; }
    void setTClass(const QString& tclass) { m_class = tclass; }

    double price() const { return m_price; }
    void setPrice(double p) { m_price = p; }

    int totalSeats() const { return m_totalSeats; }
    void setTotalSeats(int seats) { m_totalSeats = seats; }

    int remainSeats() const { return m_remainSeats; }
    void setRemainSeats(int seats) { m_remainSeats = seats; }

private:
    int m_id;
    int m_flightId;
    QString m_class;
    double m_price;
    int m_totalSeats;
    int m_remainSeats;
};

#endif // TICKET_H
