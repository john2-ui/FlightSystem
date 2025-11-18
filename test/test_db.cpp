#include <QCoreApplication>
#include <QDebug>
#include <QtSql>

// 包含实现类
#include "../dao_impl/city_dao_impl.h"
#include "../dao_impl/airplane_dao_impl.h"
#include "../dao_impl/airport_dao_impl.h"
#include "../dao_impl/flight_dao_impl.h"
#include "../dao_impl/ticket_dao_impl.h"

#include "../model/Ticket.h"
#include "../model/Airplane.h"
#include "../model/Airport.h"
#include "../model/Flight.h"
#include "../model/City.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("491122");
    db.setDatabaseName("FlightSystem");

    if (!db.open()) {
        qDebug() << "Failed to connect MySQL:" << db.lastError();
        return -1;
    }
    qDebug() << "MySQL Connected";


    // 初始化 DAO
    CityDaoImpl cityDao;
    AirportDaoImpl airportDao;
    AirplaneDaoImpl airplaneDao;
    FlightDaoImpl flightDao;
    TicketDaoImpl ticketDao;


    // ============================
    // 1. Test City
    // ============================
    qDebug() << "\n===== Test CityDao =====";

    City gz;
    gz.setName("广州");
    gz.setCode("GZ");
    gz.setCountry("中国");

    int cityId = cityDao.insert(gz);
    qDebug() << "Inserted city id =" << cityId;

    City city1 = cityDao.getById(cityId);
    qDebug() << "Query city name:" << city1.name();

    // Update
    city1.setName("广州Updated");
    cityDao.update(city1);

    City city2 = cityDao.getById(cityId);
    qDebug() << "Updated city name:" << city2.name();

    // Get all
    auto cities = cityDao.getAll();
    qDebug() << "City count =" << cities.size();

    // Delete
    cityDao.remove(cityId);
    qDebug() << "City removed. Exists?" << (cityDao.getById(cityId).id() != 0);



    // ============================
    // 2. Test Airport
    // ============================
    qDebug() << "\n===== Test AirportDao =====";

    Airport ap;
    ap.setName("白云机场");
    ap.setCode("CAN");
    ap.setCityId(cityId);   // 注意：上面 cityId 被删了，此处最好重新插入 city
    ap.setTerminalCount(2);

    int newCityId = cityDao.insert(gz);
    ap.setCityId(newCityId);

    int airportId = airportDao.insert(ap);

    Airport ap1 = airportDao.getById(airportId);
    qDebug() << "Airport name:" << ap1.name();

    // Update
    ap1.setName("白云机场Updated");
    airportDao.update(ap1);

    Airport ap2 = airportDao.getById(airportId);
    qDebug() << "Updated airport name:" << ap2.name();

    // Get all
    auto airports = airportDao.getAll();
    qDebug() << "Airport count =" << airports.size();

    // Delete
    airportDao.remove(airportId);
    qDebug() << "Airport removed. Exists?" << (airportDao.getById(airportId).id() != 0);



    // ============================
    // 3. Test Airplane
    // ============================
    qDebug() << "\n===== Test AirplaneDao =====";

    Airplane a320;
    a320.setModel("A320");
    a320.setSeatsEconomy(150);
    a320.setSeatsBusiness(20);
    a320.setSeatsFirst(10);

    int airplaneId = airplaneDao.insert(a320);

    Airplane a1 = airplaneDao.getById(airplaneId);
    qDebug() << "Airplane model:" << a1.model();

    // Update
    a1.setModel("A320-Updated");
    airplaneDao.update(a1);

    Airplane a2 = airplaneDao.getById(airplaneId);
    qDebug() << "Updated model:" << a2.model();

    // Get all
    auto planes = airplaneDao.getAll();
    qDebug() << "Airplane count =" << planes.size();

    // Delete
    airplaneDao.remove(airplaneId);
    qDebug() << "Airplane removed. Exists?" << (airplaneDao.getById(airplaneId).id() != 0);



    // ============================
    // 4. Test Flight
    // ============================
    qDebug() << "\n===== Test FlightDao =====";

    // 插入Flight需要 airportId 和 airplaneId，因此重新插入
    int cityX = cityDao.insert(gz);
    ap.setCityId(cityX);
    int airportX = airportDao.insert(ap);
    a320.setModel("A320");
    int airplaneX = airplaneDao.insert(a320);

    Flight fl;
    fl.setFlightNo("CZ3101");
    fl.setAirplaneId(airplaneX);
    fl.setDepartAirportId(airportX);
    fl.setArriveAirportId(airportX);
    fl.setDepartTime(QDateTime::currentDateTime());
    fl.setArriveTime(QDateTime::currentDateTime().addSecs(7200));
    fl.setStatus("normal");

    int flightId = flightDao.insert(fl);

    Flight fl1 = flightDao.getById(flightId);
    qDebug() << "Flight no:" << fl1.flightNo();

    // Update
    fl1.setStatus("delayed");
    flightDao.update(fl1);

    Flight fl2 = flightDao.getById(flightId);
    qDebug() << "Updated status:" << fl2.status();

    // Get all
    auto flights = flightDao.getAll();
    qDebug() << "Flight count =" << flights.size();

    // Delete
    flightDao.remove(flightId);
    qDebug() << "Flight removed. Exists?" << (flightDao.getById(flightId).id() != 0);



    // ============================
    // 5. Test Ticket
    // ============================
    qDebug() << "\n===== Test TicketDao =====";

    // 票必须依赖 flight，因此重新插入 flight
    flightId = flightDao.insert(fl);

    Ticket tk;
    tk.setFlightId(flightId);
    tk.setTClass("economy");
    tk.setPrice(899.99);
    tk.setTotalSeats(150);
    tk.setRemainSeats(120);

    int ticketId = ticketDao.insert(tk);

    Ticket tk1 = ticketDao.getById(ticketId);
    qDebug() << "Ticket class:" << tk1.tClass();

    // Update
    tk1.setPrice(999.00);
    tk1.setRemainSeats(118);
    ticketDao.update(tk1);

    Ticket tk2 = ticketDao.getById(ticketId);
    qDebug() << "Updated ticket price:" << tk2.price();

    // Get all
    auto tickets = ticketDao.getAll();
    qDebug() << "Ticket count =" << tickets.size();

    // Delete
    ticketDao.remove(ticketId);
    qDebug() << "Ticket removed. Exists?" << (ticketDao.getById(ticketId).id() != 0);


    qDebug() << "\n===== ALL TEST DONE =====";
    return 0;
}
