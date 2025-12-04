#include <QCoreApplication>
#include <QDebug>
#include <QtSql>

// 包含实现类
#include "../dao_impl/city_dao_impl.h"
#include "../dao_impl/airplane_dao_impl.h"
#include "../dao_impl/airport_dao_impl.h"
#include "../dao_impl/flight_dao_impl.h"
#include "../dao_impl/ticket_dao_impl.h"
#include "../dao_impl/user_dao_impl.h"

#include "../model/Ticket.h"
#include "../model/Airplane.h"
#include "../model/Airport.h"
#include "../model/Flight.h"
#include "../model/City.h"
#include "../model/User.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // 初始化 DAO
    CityDaoImpl cityDao;
    AirportDaoImpl airportDao;
    AirplaneDaoImpl airplaneDao;
    FlightDaoImpl flightDao;
    TicketDaoImpl ticketDao;
    UserDaoImpl userDao;

    qDebug() << "===== START CRUD TEST =====";

    // ============================
    // 1. Insert City
    // ============================
    City gz;
    gz.setName("广州");
    gz.setCode("GZ");
    gz.setCountry("中国");
    int cityId = cityDao.insert(gz);
    qDebug() << "Inserted city id =" << cityId;

    // Query
    City city = cityDao.getById(cityId);
    qDebug() << "Query city:" << city.name();

    // Update
    city.setName("广州Updated");
    cityDao.update(city);
    qDebug() << "Updated city name:" << cityDao.getById(cityId).name();

    // ============================
    // 2. Insert Airport
    // ============================
    Airport ap;
    ap.setName("白云机场");
    ap.setCode("CAN");
    ap.setCityId(cityId);
    ap.setTerminalCount(2);
    int airportId = airportDao.insert(ap);
    qDebug() << "Inserted airport id =" << airportId;

    Airport airport = airportDao.getById(airportId);
    qDebug() << "Query airport:" << airport.name();

    airport.setName("白云机场Updated");
    airportDao.update(airport);
    qDebug() << "Updated airport name:" << airportDao.getById(airportId).name();

    // ============================
    // 3. Insert Airplane
    // ============================
    Airplane plane;
    plane.setModel("A320");
    plane.setSeatsEconomy(150);
    plane.setSeatsBusiness(20);
    plane.setSeatsFirst(10);
    int airplaneId = airplaneDao.insert(plane);
    qDebug() << "Inserted airplane id =" << airplaneId;

    Airplane a = airplaneDao.getById(airplaneId);
    qDebug() << "Query airplane model:" << a.model();

    a.setModel("A320-Updated");
    airplaneDao.update(a);
    qDebug() << "Updated airplane model:" << airplaneDao.getById(airplaneId).model();

    // ============================
    // 4. Insert Flight
    // ============================
    Flight fl;
    fl.setFlightNo("CZ3101");
    fl.setAirplaneId(airplaneId);
    fl.setDepartAirportId(airportId);
    fl.setArriveAirportId(airportId);
    fl.setDepartTime(QDateTime::currentDateTime());
    fl.setArriveTime(QDateTime::currentDateTime().addSecs(7200));
    fl.setStatus("normal");
    int flightId = flightDao.insert(fl);
    qDebug() << "Inserted flight id =" << flightId;

    Flight f = flightDao.getById(flightId);
    qDebug() << "Query flight no:" << f.flightNo();

    f.setStatus("delayed");
    flightDao.update(f);
    qDebug() << "Updated flight status:" << flightDao.getById(flightId).status();

    // ============================
    // 5. Insert Ticket
    // ============================
    Ticket tk;
    tk.setFlightId(flightId);
    tk.setTClass("economy");
    tk.setPrice(899.99);
    tk.setTotalSeats(150);
    tk.setRemainSeats(120);
    int ticketId = ticketDao.insert(tk);
    qDebug() << "Inserted ticket id =" << ticketId;

    Ticket t = ticketDao.getById(ticketId);
    qDebug() << "Query ticket price:" << t.price();

    t.setPrice(999.0);
    t.setRemainSeats(118);
    ticketDao.update(t);
    qDebug() << "Updated ticket price:" << ticketDao.getById(ticketId).price();


    // ============================
    // 6. Insert User
    // ============================
    QVector<int> userTickets;
    userTickets.append(ticketId); // 假设用户购买了刚才插入的票

    User u("tom", "123456", userTickets, 0);
    int userId = userDao.insert(u);
    qDebug() << "Inserted user id =" << userId;

    // Query
    User user = userDao.getById(userId);
    qDebug() << "Query user username:" << user.username()
            << ", ticketsID:" << user.ticketsID()
            << ", isSuper:" << user.isSuper();

    // Update
    user.setPassword("654321");
    user.setTicketsID({ticketId, ticketId}); // 用户买了两张票
    userDao.update(user);
    User updatedUser = userDao.getById(userId);
    qDebug() << "Updated user password:" << updatedUser.password()
            << ", ticketsID:" << updatedUser.ticketsID()
            << ", isSuper:" << user.isSuper();

    // Query by username
    User userByName = userDao.getByUsername("tom");
    qDebug() << "Query by username, id:" << userByName.id()
            << ", ticketsID:" << userByName.ticketsID()
            << ", isSuper:" << user.isSuper();


    // ============================
    // 7. Delete all in reverse dependency order
    // ============================
    qDebug() << "\n===== Delete records =====";

    // 1. Ticket first
    ticketDao.remove(ticketId);
    qDebug() << "Ticket removed. Exists?" << (ticketDao.getById(ticketId).id() != 0);

    // 2. Flight
    flightDao.remove(flightId);
    qDebug() << "Flight removed. Exists?" << (flightDao.getById(flightId).id() != 0);

    // 3. Airplane
    airplaneDao.remove(airplaneId);
    qDebug() << "Airplane removed. Exists?" << (airplaneDao.getById(airplaneId).id() != 0);

    // 4. Airport
    airportDao.remove(airportId);
    qDebug() << "Airport removed. Exists?" << (airportDao.getById(airportId).id() != 0);

    // 5. City
    cityDao.remove(cityId);
    qDebug() << "City removed. Exists?" << (cityDao.getById(cityId).id() != 0);

    // 6. User
    userDao.remove(userId);
    qDebug() << "User removed. Exists?" << (userDao.getById(userId).id() != 0);

    qDebug() << "\n===== ALL TEST DONE =====";

    return 0;
}
