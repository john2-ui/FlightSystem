#ifndef FLIGHT_H
#define FLIGHT_H

#include <QString>
#include <QDateTime>

class Flight {
public:
    Flight() = default;
    Flight(int id, const QString& flightNo, int airplaneId, int departAirportId, int arriveAirportId,
           const QDateTime& departTime, const QDateTime& arriveTime, const QString& status)
        : m_id(id), m_flightNo(flightNo), m_airplaneId(airplaneId), m_departAirportId(departAirportId),
        m_arriveAirportId(arriveAirportId), m_departTime(departTime), m_arriveTime(arriveTime), m_status(status) {}

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString flightNo() const { return m_flightNo; }
    void setFlightNo(const QString& no) { m_flightNo = no; }

    int airplaneId() const { return m_airplaneId; }
    void setAirplaneId(int id) { m_airplaneId = id; }

    int departAirportId() const { return m_departAirportId; }
    void setDepartAirportId(int id) { m_departAirportId = id; }

    int arriveAirportId() const { return m_arriveAirportId; }
    void setArriveAirportId(int id) { m_arriveAirportId = id; }

    QDateTime departTime() const { return m_departTime; }
    void setDepartTime(const QDateTime& time) { m_departTime = time; }

    QDateTime arriveTime() const { return m_arriveTime; }
    void setArriveTime(const QDateTime& time) { m_arriveTime = time; }

    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }

private:
    int m_id;
    QString m_flightNo;
    int m_airplaneId;
    int m_departAirportId;
    int m_arriveAirportId;
    QDateTime m_departTime;
    QDateTime m_arriveTime;
    QString m_status;
};

#endif // FLIGHT_H
