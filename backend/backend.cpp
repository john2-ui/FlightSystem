#include "backend.h"
#include "../db/DBManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Backend& Backend::instance() {
    static Backend instance;
    return instance;
}

Backend::Backend() {
    qDebug() << "Backend 初始化...";

    cityDao = new CityDaoImpl();
    airportDao = new AirportDaoImpl();
    airplaneDao = new AirplaneDaoImpl();
    flightDao = new FlightDaoImpl();
    ticketDao = new TicketDaoImpl();
    userDao = new UserDaoImpl();
    qDebug() << "Backend 初始化完成";
}

Backend::~Backend() {
    delete cityDao;
    delete airportDao;
    delete airplaneDao;
    delete flightDao;
    delete ticketDao;
    delete userDao;
}

QList<City> Backend::getAllCities() {
    return cityDao->getAll();
}

City Backend::getCityById(int id) {
    return cityDao->getById(id);
}

City Backend::getCityByCode(const QString& code) {
    QList<City> cities = cityDao->getAll();
    for (const City& city : cities) {
        if (city.code() == code) {
            return city;
        }
    }
    return City();
}

QList<Airport> Backend::getAllAirports() {
    return airportDao->getAll();
}

Airport Backend::getAirportById(int id) {
    return airportDao->getById(id);
}

Airport Backend::getAirportByCode(const QString& code) {
    QList<Airport> airports = airportDao->getAll();
    for (const Airport& airport : airports) {
        if (airport.code() == code) {
            return airport;
        }
    }
    return Airport();
}

QList<Airport> Backend::getAirportsByCity(int cityId) {
    QList<Airport> result;
    QList<Airport> allAirports = airportDao->getAll();
    for (const Airport& airport : allAirports) {
        if (airport.cityId() == cityId) {
            result.append(airport);
        }
    }
    return result;
}

QList<Airplane> Backend::getAllAirplanes() {
    return airplaneDao->getAll();
}

Airplane Backend::getAirplaneById(int id) {
    return airplaneDao->getById(id);
}

FlightDetailInfo Backend::getFlightDetail(int flightId) {
    FlightDetailInfo info;

    Flight flight = flightDao->getById(flightId);
    if (flight.id() == 0) {
        qDebug() << "航班不存在:" << flightId;
        return info;
    }

    info.flightId = flight.id();
    info.flightNo = flight.flightNo();
    info.departTime = flight.departTime();
    info.arriveTime = flight.arriveTime();
    info.status = flight.status();

    Airport departAirport = airportDao->getById(flight.departAirportId());
    if (departAirport.id() != 0) {
        City departCity = cityDao->getById(departAirport.cityId());
        info.departCityName = departCity.name();
        info.departAirportName = departAirport.name();
        info.departAirportCode = departAirport.code();
    }

    Airport arriveAirport = airportDao->getById(flight.arriveAirportId());
    if (arriveAirport.id() != 0) {
        City arriveCity = cityDao->getById(arriveAirport.cityId());
        info.arriveCityName = arriveCity.name();
        info.arriveAirportName = arriveAirport.name();
        info.arriveAirportCode = arriveAirport.code();
    }

    Airplane airplane = airplaneDao->getById(flight.airplaneId());
    if (airplane.id() != 0) {
        info.airplaneModel = airplane.model();
    }

    // TODO: 优化 - 应使用 getByFlightId() 避免全表扫描
    QList<Ticket> allTickets = ticketDao->getAll();
    for (const Ticket& ticket : allTickets) {
        if (ticket.flightId() == flightId) {
            TicketInfo tInfo;
            tInfo.ticketId = ticket.id();
            tInfo.ticketClass = ticket.tClass();
            tInfo.price = ticket.price();
            tInfo.totalSeats = ticket.totalSeats();
            tInfo.remainSeats = ticket.remainSeats();
            info.tickets[ticket.tClass()] = tInfo;
        }
    }

    return info;
}

QList<FlightDetailInfo> Backend::getAllFlights() {
    QList<FlightDetailInfo> result;
    QList<Flight> flights = flightDao->getAll();

    for (const Flight& flight : flights) {
        result.append(getFlightDetail(flight.id()));
    }

    return result;
}

QList<FlightDetailInfo> Backend::searchFlights(
    const QString& fromCityCode,
    const QString& toCityCode,
    const QDate& date
    ) {
    QList<FlightDetailInfo> result;

    City fromCity = getCityByCode(fromCityCode);
    City toCity = getCityByCode(toCityCode);

    if (fromCity.id() == 0 || toCity.id() == 0) {
        qDebug() << "城市代码无效:" << fromCityCode << toCityCode;
        return result;
    }

    QList<Airport> fromAirports = getAirportsByCity(fromCity.id());
    QList<Airport> toAirports = getAirportsByCity(toCity.id());

    // TODO: 严重性能问题 - 应使用 SQL JOIN 一次查询完成
    QList<Flight> allFlights = flightDao->getAll();
    for (const Flight& flight : allFlights) {
        if (flight.departTime().date() != date) {
            continue;
        }

        bool matchDepart = false;
        bool matchArrive = false;

        for (const Airport& airport : fromAirports) {
            if (flight.departAirportId() == airport.id()) {
                matchDepart = true;
                break;
            }
        }

        for (const Airport& airport : toAirports) {
            if (flight.arriveAirportId() == airport.id()) {
                matchArrive = true;
                break;
            }
        }

        if (matchDepart && matchArrive) {
            result.append(getFlightDetail(flight.id()));
        }
    }

    qDebug() << "查询航班:" << fromCityCode << "->" << toCityCode
             << "日期:" << date << "结果:" << result.size();

    return result;
}

bool Backend::checkTicketAvailability(
    int flightId,
    const QString& ticketClass,
    int quantity
    ) {
    // TODO: 优化 - 应使用 getByFlightAndClass() 避免全表扫描
    QList<Ticket> allTickets = ticketDao->getAll();
    for (const Ticket& ticket : allTickets) {
        if (ticket.flightId() == flightId && ticket.tClass() == ticketClass) {
            return ticket.remainSeats() >= quantity;
        }
    }
    return false;
}

bool Backend::bookTicket(
    int flightId,
    const QString& ticketClass,
    int quantity,
    QString& errorMsg
    ) {
    QSqlDatabase db = DBManager::instance().db();

    if (!db.transaction()) {
        errorMsg = "无法开启事务";
        qDebug() << errorMsg;
        return false;
    }

    Flight flight = flightDao->getById(flightId);
    if (flight.id() == 0) {
        db.rollback();
        errorMsg = "航班不存在";
        qDebug() << errorMsg;
        return false;
    }

    if (flight.status() != "normal") {
        db.rollback();
        errorMsg = "航班状态异常: " + flight.status();
        qDebug() << errorMsg;
        return false;
    }

    Ticket ticket;
    QList<Ticket> allTickets = ticketDao->getAll();
    bool found = false;
    for (const Ticket& t : allTickets) {
        if (t.flightId() == flightId && t.tClass() == ticketClass) {
            ticket = t;
            found = true;
            break;
        }
    }

    if (!found) {
        db.rollback();
        errorMsg = "该舱位不存在";
        qDebug() << errorMsg;
        return false;
    }

    if (ticket.remainSeats() < quantity) {
        db.rollback();
        errorMsg = QString("余票不足，仅剩 %1 张").arg(ticket.remainSeats());
        qDebug() << errorMsg;
        return false;
    }

    ticket.setRemainSeats(ticket.remainSeats() - quantity);
    if (!ticketDao->update(ticket)) {
        db.rollback();
        errorMsg = "更新余票失败";
        qDebug() << errorMsg;
        return false;
    }

    if (!db.commit()) {
        db.rollback();
        errorMsg = "提交事务失败";
        qDebug() << errorMsg;
        return false;
    }

    qDebug() << "预订成功：航班" << flight.flightNo()
             << "舱位" << ticketClass
             << "数量" << quantity
             << "剩余" << ticket.remainSeats();

    return true;
}

bool Backend::cancelBooking(
    int flightId,
    const QString& ticketClass,
    int quantity,
    QString& errorMsg
    ) {
    QSqlDatabase db = DBManager::instance().db();

    if (!db.transaction()) {
        errorMsg = "无法开启事务";
        return false;
    }

    Ticket ticket;
    QList<Ticket> allTickets = ticketDao->getAll();
    bool found = false;
    for (const Ticket& t : allTickets) {
        if (t.flightId() == flightId && t.tClass() == ticketClass) {
            ticket = t;
            found = true;
            break;
        }
    }

    if (!found) {
        db.rollback();
        errorMsg = "该舱位不存在";
        return false;
    }

    if (ticket.remainSeats() + quantity > ticket.totalSeats()) {
        db.rollback();
        errorMsg = "取消数量超出范围";
        return false;
    }

    ticket.setRemainSeats(ticket.remainSeats() + quantity);
    if (!ticketDao->update(ticket)) {
        db.rollback();
        errorMsg = "更新余票失败";
        return false;
    }

    if (!db.commit()) {
        db.rollback();
        errorMsg = "提交事务失败";
        return false;
    }

    qDebug() << "取消预订成功：数量" << quantity << "剩余" << ticket.remainSeats();
    return true;
}

int Backend::addFlight(
    const QString& flightNo,
    int airplaneId,
    int departAirportId,
    int arriveAirportId,
    const QDateTime& departTime,
    const QDateTime& arriveTime,
    const QString& status,
    double priceEconomy,
    double priceBusiness,
    double priceFirst,
    QString& errorMsg
) {
    QSqlDatabase db = DBManager::instance().db();
    if (!db.transaction()) {
        errorMsg = "无法开启事务";
        qDebug() << errorMsg;
        return -1;
    }

    Airplane airplane = airplaneDao->getById(airplaneId);
    if (airplane.id() == 0) {
        db.rollback();
        errorMsg = "飞机不存在";
        qDebug() << errorMsg;
        return -1;
    }

    Airport departAirport = airportDao->getById(departAirportId);
    Airport arriveAirport = airportDao->getById(arriveAirportId);
    if (departAirport.id() == 0 || arriveAirport.id() == 0)/*默认空对象为0*/ {
        db.rollback();
        errorMsg = "机场不存在";
        qDebug() << errorMsg;
        return -1;
    }

    Flight flight(0, flightNo, airplaneId, departAirportId, arriveAirportId, departTime, arriveTime, status);
    int flightId = flightDao->insert(flight);
    if (flightId <= 0) {
        db.rollback();
        errorMsg = "插入航班失败";
        qDebug() << errorMsg;
        return -1;
    }

    const struct TicketSeed {
        QString klass;
        int seats;
        double price;
    } ticketSeeds[] = {
        {QStringLiteral("economy"), airplane.seatsEconomy(), priceEconomy},
        {QStringLiteral("business"), airplane.seatsBusiness(), priceBusiness},
        {QStringLiteral("first"), airplane.seatsFirst(), priceFirst}
    };

    for (const TicketSeed& seed : ticketSeeds) {
        if (seed.seats <= 0) {
            continue;
        }
        if (seed.price < 0.0) {
            db.rollback();
            errorMsg = QStringLiteral("%1舱位票价不能为负数").arg(seed.klass);
            qDebug() << errorMsg;
            return -1;
        }
        Ticket ticket(0, flightId, seed.klass, seed.price, seed.seats, seed.seats);
        if (ticketDao->insert(ticket) <= 0) {
            db.rollback();
            errorMsg = QStringLiteral("初始化%1舱位失败").arg(seed.klass);
            qDebug() << errorMsg;
            return -1;
        }
    }

    if (!db.commit()) {
        db.rollback();
        errorMsg = "提交事务失败";
        qDebug() << errorMsg;
        return -1;
    }

    qDebug() << "添加航班成功，ID:" << flightId;
    return flightId;
}

bool Backend::updateFlight(
    const Flight& flight,
    double priceEconomy,
    double priceBusiness,
    double priceFirst,
    QString& errorMsg
) {
    QSqlDatabase db = DBManager::instance().db();
    if (!db.transaction()) {
        errorMsg = "无法开启事务";
        qDebug() << errorMsg;
        return false;
    }

    Airplane airplane = airplaneDao->getById(flight.airplaneId());
    if (airplane.id() == 0) {
        db.rollback();
        errorMsg = "飞机不存在";
        qDebug() << errorMsg;
        return false;
    }

    Airport departAirport = airportDao->getById(flight.departAirportId());
    Airport arriveAirport = airportDao->getById(flight.arriveAirportId());
    if (departAirport.id() == 0 || arriveAirport.id() == 0) {
        db.rollback();
        errorMsg = "机场不存在";
        qDebug() << errorMsg;
        return false;
    }

    if (!flightDao->update(flight)) {
        db.rollback();
        errorMsg = "更新航班失败";
        qDebug() << errorMsg;
        return false;
    }

    const struct TicketSeed {
        QString klass;
        int seats;
        double price;
    } ticketSeeds[] = {
        {QStringLiteral("economy"), airplane.seatsEconomy(), priceEconomy},
        {QStringLiteral("business"), airplane.seatsBusiness(), priceBusiness},
        {QStringLiteral("first"), airplane.seatsFirst(), priceFirst}
    };

    QList<Ticket> allTickets = ticketDao->getAll();
    QMap<QString, Ticket> ticketByClass;
    for (const Ticket& ticket : allTickets) {
        if (ticket.flightId() == flight.id()) {
            ticketByClass.insert(ticket.tClass(), ticket);
        }
    }

    for (const TicketSeed& seed : ticketSeeds) {
        const bool hasTicket = ticketByClass.contains(seed.klass);

        if (seed.seats <= 0) {
            if (hasTicket) {
                const Ticket existing = ticketByClass.value(seed.klass);
                const int sold = existing.totalSeats() - existing.remainSeats();
                if (sold > 0) {
                    db.rollback();
                    errorMsg = QStringLiteral("%1舱位已有售票，无法移除").arg(seed.klass);
                    qDebug() << errorMsg;
                    return false;
                }
                if (!ticketDao->remove(existing.id())) {
                    db.rollback();
                    errorMsg = QStringLiteral("移除%1舱位失败").arg(seed.klass);
                    qDebug() << errorMsg;
                    return false;
                }
            }
            continue;
        }

        if (hasTicket) {
            Ticket existing = ticketByClass.value(seed.klass);
            const int sold = existing.totalSeats() - existing.remainSeats();
            if (sold > seed.seats) {
                db.rollback();
                errorMsg = QStringLiteral("%1舱位已售%2张，少于新座位数").arg(seed.klass).arg(sold);
                qDebug() << errorMsg;
                return false;
            }
            const int newRemain = seed.seats - sold;
            existing.setTotalSeats(seed.seats);
            existing.setRemainSeats(newRemain);
            if (seed.price >= 0.0) {
                existing.setPrice(seed.price);
            }
            if (!ticketDao->update(existing)) {
                db.rollback();
                errorMsg = QStringLiteral("更新%1舱位失败").arg(seed.klass);
                qDebug() << errorMsg;
                return false;
            }
        } else {
            if (seed.price < 0.0) {
                db.rollback();
                errorMsg = QStringLiteral("%1舱位票价不能为负数").arg(seed.klass);
                qDebug() << errorMsg;
                return false;
            }
            Ticket ticket(0, flight.id(), seed.klass, seed.price, seed.seats, seed.seats);
            if (ticketDao->insert(ticket) <= 0) {
                db.rollback();
                errorMsg = QStringLiteral("初始化%1舱位失败").arg(seed.klass);
                qDebug() << errorMsg;
                return false;
            }
        }
    }

    if (!db.commit()) {
        db.rollback();
        errorMsg = "提交事务失败";
        qDebug() << errorMsg;
        return false;
    }

    return true;
}

bool Backend::deleteFlight(int flightId, QString& errorMsg) {
    if (!flightDao->remove(flightId)) {
        errorMsg = "删除航班失败";
        return false;
    }
    return true;
}

bool Backend::updateFlightStatus(int flightId, const QString& status) {
    Flight flight = flightDao->getById(flightId);
    if (flight.id() == 0) {
        return false;
    }
    flight.setStatus(status);
    return flightDao->update(flight);
}

int Backend::addAirplane(const QString& model, int seatsEconomy, int seatsBusiness, int seatsFirst) {
    Airplane airplane(0, model, seatsEconomy, seatsBusiness, seatsFirst);
    return airplaneDao->insert(airplane);
}

bool Backend::updateAirplane(const Airplane& airplane) {
    return airplaneDao->update(airplane);
}

bool Backend::deleteAirplane(int airplaneId) {
    return airplaneDao->remove(airplaneId);
}

int Backend::addCity(const QString& name, const QString& code, const QString& country) {
    City city(0, name, code, country);
    return cityDao->insert(city);
}

int Backend::addAirport(const QString& name, const QString& code, int cityId, int terminalCount) {
    Airport airport(0, name, code, cityId, terminalCount);
    return airportDao->insert(airport);
}

bool Backend::registerUser(const QString& username, const QString& password, bool isAdmin, QString& errorMsg) {
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        errorMsg = "用户名或密码不能为空";
        return false;
    }

    User existing = userDao->getByUsername(username);
    if (existing.id() > 0) {
        errorMsg = "用户名已存在";
        return false;
    }

    QVector<int> emptyTickets;
    User user(username, password, emptyTickets, isAdmin ? 1 : 0);
    int userId = userDao->insert(user);
    if (userId <= 0) {
        errorMsg = "注册失败";
        return false;
    }
    return true;
}

bool Backend::loginUser(const QString& username, const QString& password, int& userId, bool& isAdmin, QString& errorMsg) {
    userId = -1;
    isAdmin = false;
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        errorMsg = "用户名或密码不能为空";
        return false;
    }

    User user = userDao->getByUsername(username);
    if (user.id() <= 0) {
        errorMsg = "用户不存在";
        return false;
    }

    if (user.isSuper() < 0) {
        errorMsg = "账号已注销";
        return false;
    }

    if (user.password() != password) {
        errorMsg = "密码错误";
        return false;
    }

    userId = user.id();
    isAdmin = (user.isSuper() == 1);
    return true;
}

bool Backend::deleteUser(int userId, QString& errorMsg) {
    if (userId <= 0) {
        errorMsg = "无效的用户ID";
        return false;
    }
    User user = userDao->getById(userId);
    if (user.id() <= 0) {
        errorMsg = "用户不存在";
        return false;
    }

    if (user.isSuper() == 1) {
        errorMsg = "管理员账号禁止注销";
        return false;
    }

    const QString tombStone = QStringLiteral("%1#deleted_%2")
                                  .arg(user.username())
                                  .arg(QDateTime::currentMSecsSinceEpoch());
    user.setUsername(tombStone);
    user.setPassword(QStringLiteral(""));
    user.setTicketsID({-1});
    user.setIsSuper(-1);

    if (!userDao->update(user)) {
        errorMsg = "注销用户失败";
        return false;
    }
    return true;
}

bool Backend::purchaseTicket(int userId, int ticketId, int quantity, QString& errorMsg) {
    if (userId <= 0 || ticketId <= 0 || quantity <= 0) {
        errorMsg = "参数无效";
        return false;
    }

    User user = userDao->getById(userId);
    if (user.id() <= 0 || user.isSuper() < 0) {
        errorMsg = "用户不存在";
        qDebug()<<"用户验证失败：id="<<user.id()<<",isSuper"<<user.isSuper();
        return false;
    }

    Ticket ticket = ticketDao->getById(ticketId);
    if (ticket.id() <= 0) {
        errorMsg = "票不存在";
        return false;
    }

    Flight flight = flightDao->getById(ticket.flightId());
    if (flight.id() <= 0) {
        errorMsg = "关联航班不存在";
        return false;
    }

    if (!checkTicketAvailability(flight.id(), ticket.tClass(), quantity)) {
        errorMsg = "余票不足";
        return false;
    }

    QString bookError;
    if (!bookTicket(flight.id(), ticket.tClass(), quantity, bookError)) {
        errorMsg = bookError;
        return false;
    }

    QVector<int> tickets = user.ticketsID();
    for (int i = 0; i < quantity; ++i) {
        tickets.append(ticket.id());
    }
    user.setTicketsID(tickets);

    if (!userDao->update(user)) {
        QString rollbackError;
        cancelBooking(flight.id(), ticket.tClass(), quantity, rollbackError);
        errorMsg = "更新用户票信息失败";
        return false;
    }

    return true;
}
//同一人购买同一仓位多张票的ticketId是相同的
bool Backend::refundTicket(int userId, int ticketId, int quantity, QString& errorMsg) {
    if (userId <= 0 || ticketId <= 0 || quantity <= 0) {
        errorMsg = "参数无效";
        return false;
    }

    User user = userDao->getById(userId);
    if (user.id() <= 0 || user.isSuper() < 0) {
        errorMsg = "用户不存在";
        return false;
    }

    Ticket ticket = ticketDao->getById(ticketId);
    if (ticket.id() <= 0) {
        errorMsg = "票不存在";
        return false;
    }

    QVector<int> tickets = user.ticketsID();
    int owned = 0;
    for (int id : tickets) {
        if (id == ticketId) {
            ++owned;
        }
    }

    if (owned < quantity) {
        errorMsg = "用户没有足够的票可退";
        return false;
    }

    QString refundError;
    if (!cancelBooking(ticket.flightId(), ticket.tClass(), quantity, refundError)) {
        errorMsg = refundError;
        return false;
    }

    int removed = 0;
    QVector<int> updated;
    updated.reserve(tickets.size() - quantity);
    for (int id : tickets) {
        if (id == ticketId && removed < quantity) {
            ++removed;
            continue;
        }
        updated.append(id);
    }
    user.setTicketsID(updated);

    if (!userDao->update(user)) {
        QString revertErr;
        bookTicket(ticket.flightId(), ticket.tClass(), quantity, revertErr);
        errorMsg = "更新用户票信息失败";
        return false;
    }

    return true;
}

User Backend::getUserById(int userId){
    return userDao->getById(userId);
}

User Backend::getUserByname(const QString& username){
    return userDao->getByUsername(username);
}

Ticket Backend::getTicketById(int ticketId){
    return ticketDao->getById(ticketId);
}
