#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDate>
#include <QDateTime>
#include <QMap>
#include "../dao_impl/city_dao_impl.h"
#include "../dao_impl/airport_dao_impl.h"
#include "../dao_impl/airplane_dao_impl.h"
#include "../dao_impl/flight_dao_impl.h"
#include "../dao_impl/ticket_dao_impl.h"

struct TicketInfo {
    int ticketId;
    QString ticketClass;
    double price;
    int totalSeats;
    int remainSeats;
    bool available() const { return remainSeats > 0; }
};

struct FlightDetailInfo {
    int flightId;
    QString flightNo;
    
    QString departCityName;
    QString departAirportName;
    QString departAirportCode;
    QDateTime departTime;
    
    QString arriveCityName;
    QString arriveAirportName;
    QString arriveAirportCode;
    QDateTime arriveTime;
    
    QString airplaneModel;
    QString status;
    
    QMap<QString, TicketInfo> tickets;
};

class Backend
{
public:
    static Backend& instance();
     
    QList<City> getAllCities();
    City getCityById(int id);
    City getCityByCode(const QString& code);
    
    QList<Airport> getAllAirports();
    Airport getAirportById(int id);
    Airport getAirportByCode(const QString& code);
    QList<Airport> getAirportsByCity(int cityId);
    
    QList<Airplane> getAllAirplanes();
    Airplane getAirplaneById(int id);
    
    QList<FlightDetailInfo> searchFlights(
        const QString& fromCityCode,
        const QString& toCityCode,
        const QDate& date
    );
    
    FlightDetailInfo getFlightDetail(int flightId);
    QList<FlightDetailInfo> getAllFlights();
    
    bool checkTicketAvailability(
        int flightId,
        const QString& ticketClass,
        int quantity = 1
    );
    
    bool bookTicket(
        int flightId,
        const QString& ticketClass,
        int quantity,
        QString& errorMsg
    );
    
    bool cancelBooking(
        int flightId,
        const QString& ticketClass,
        int quantity,
        QString& errorMsg
    );
    
    int addFlight(const Flight& flight, QString& errorMsg);
    bool updateFlight(const Flight& flight, QString& errorMsg);
    bool deleteFlight(int flightId, QString& errorMsg);
    bool updateFlightStatus(int flightId, const QString& status);
    
    int addAirplane(const Airplane& airplane);
    bool updateAirplane(const Airplane& airplane);
    bool deleteAirplane(int airplaneId);
    
    int addCity(const City& city);
    int addAirport(const Airport& airport);

private:
    Backend();
    ~Backend();
    
    CityDaoImpl* cityDao;
    AirportDaoImpl* airportDao;
    AirplaneDaoImpl* airplaneDao;
    FlightDaoImpl* flightDao;
    TicketDaoImpl* ticketDao;
};

#endif // BACKEND_H
