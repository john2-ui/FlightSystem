#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include "../backend/backend.h"

void testBasicQueries();
void testFlightSearch();
void testBooking();
void testAdminFunctions();
void testAddFlightAPI();

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "\n========================================";
    qDebug() << "  Backend 功能测试";
    qDebug() << "========================================\n";

    // 测试1: 基础数据查询
    testBasicQueries();
    
    // 测试2: 航班查询
    testFlightSearch();
    
    // 测试3: 机票预订
    testBooking();
    
    // 测试4: 管理员功能
    testAdminFunctions();

    // 测试5: 新增航班接口（字段入参版）
    testAddFlightAPI();

    qDebug() << "\n========================================";
    qDebug() << "  所有测试完成！";
    qDebug() << "========================================\n";

    return 0;
}

void testBasicQueries() {
    qDebug() << "\n===== 测试1: 基础数据查询 =====";
    
    Backend& backend = Backend::instance();
    
    // 1.1 查询所有城市
    qDebug() << "\n--- 查询所有城市 ---";
    QList<City> cities = backend.getAllCities();
    qDebug() << "城市数量:" << cities.size();
    for (const City& city : cities) {
        qDebug() << "  -" << city.name() << "(" << city.code() << ")";
    }
    
    // 1.2 查询所有机场
    qDebug() << "\n--- 查询所有机场 ---";
    QList<Airport> airports = backend.getAllAirports();
    qDebug() << "机场数量:" << airports.size();
    for (const Airport& airport : airports) {
        qDebug() << "  -" << airport.name() << "(" << airport.code() << ")";
    }
    
    // 1.3 查询所有飞机
    qDebug() << "\n--- 查询所有飞机 ---";
    QList<Airplane> airplanes = backend.getAllAirplanes();
    qDebug() << "飞机数量:" << airplanes.size();
    for (const Airplane& airplane : airplanes) {
        qDebug() << "  -" << airplane.model() 
                 << "经济舱:" << airplane.seatsEconomy()
                 << "商务舱:" << airplane.seatsBusiness()
                 << "头等舱:" << airplane.seatsFirst();
    }
}

void testFlightSearch() {
    qDebug() << "\n===== 测试2: 航班查询 =====";
    
    Backend& backend = Backend::instance();
    
    // 2.1 查询所有航班
    qDebug() << "\n--- 查询所有航班 ---";
    QList<FlightDetailInfo> allFlights = backend.getAllFlights();
    qDebug() << "航班总数:" << allFlights.size();
    
    for (const FlightDetailInfo& info : allFlights) {
        qDebug() << "\n航班号:" << info.flightNo;
        qDebug() << "  出发:" << info.departCityName << info.departAirportName 
                 << "(" << info.departAirportCode << ")"
                 << info.departTime.toString("yyyy-MM-dd hh:mm");
        qDebug() << "  到达:" << info.arriveCityName << info.arriveAirportName 
                 << "(" << info.arriveAirportCode << ")"
                 << info.arriveTime.toString("yyyy-MM-dd hh:mm");
        qDebug() << "  机型:" << info.airplaneModel;
        qDebug() << "  状态:" << info.status;
        
        qDebug() << "  机票信息:";
        for (auto it = info.tickets.begin(); it != info.tickets.end(); ++it) {
            const TicketInfo& ticket = it.value();
            qDebug() << "    -" << ticket.ticketClass 
                     << "价格:" << ticket.price 
                     << "余票:" << ticket.remainSeats << "/" << ticket.totalSeats;
        }
    }
    
    // 2.2 按航线查询航班（如果有数据的话）
    if (!allFlights.isEmpty()) {
        qDebug() << "\n--- 测试航线查询 ---";
        // 这里需要实际的城市代码，暂时跳过
        // QList<FlightDetailInfo> results = backend.searchFlights("GZ", "BJ", QDate::currentDate());
        // qDebug() << "查询结果:" << results.size();
    }
}

void testBooking() {
    qDebug() << "\n===== 测试3: 机票预订 =====";
    
    Backend& backend = Backend::instance();
    
    // 获取第一个航班用于测试
    QList<FlightDetailInfo> flights = backend.getAllFlights();
    if (flights.isEmpty()) {
        qDebug() << "没有航班数据，跳过预订测试";
        return;
    }
    
    FlightDetailInfo testFlight = flights.first();
    qDebug() << "\n使用航班:" << testFlight.flightNo << "进行测试";
    
    if (testFlight.tickets.isEmpty()) {
        qDebug() << "该航班没有机票数据，跳过预订测试";
        return;
    }
    
    // 获取第一个舱位
    QString ticketClass = testFlight.tickets.keys().first();
    TicketInfo ticketInfo = testFlight.tickets[ticketClass];
    
    qDebug() << "\n测试舱位:" << ticketClass;
    qDebug() << "预订前余票:" << ticketInfo.remainSeats;
    
    // 3.1 测试余票检查
    qDebug() << "\n--- 测试余票检查 ---";
    bool available = backend.checkTicketAvailability(testFlight.flightId, ticketClass, 1);
    qDebug() << "余票充足:" << (available ? "是" : "否");
    
    // 3.2 测试预订
    qDebug() << "\n--- 测试预订机票 ---";
    QString errorMsg;
    bool success = backend.bookTicket(testFlight.flightId, ticketClass, 2, errorMsg);
    
    if (success) {
        qDebug() << "预订成功！";
        // 查询预订后的余票
        FlightDetailInfo updatedFlight = backend.getFlightDetail(testFlight.flightId);
        qDebug() << "预订后余票:" << updatedFlight.tickets[ticketClass].remainSeats;
    } else {
        qDebug() << "预订失败:" << errorMsg;
    }
    
    // 3.3 测试取消预订
    if (success) {
        qDebug() << "\n--- 测试取消预订 ---";
        bool cancelSuccess = backend.cancelBooking(testFlight.flightId, ticketClass, 2, errorMsg);
        
        if (cancelSuccess) {
            qDebug() << "取消成功！";
            FlightDetailInfo updatedFlight = backend.getFlightDetail(testFlight.flightId);
            qDebug() << "取消后余票:" << updatedFlight.tickets[ticketClass].remainSeats;
        } else {
            qDebug() << "取消失败:" << errorMsg;
        }
    }
    
    // 3.4 测试余票不足的情况
    qDebug() << "\n--- 测试余票不足 ---";
    success = backend.bookTicket(testFlight.flightId, ticketClass, 999999, errorMsg);
    qDebug() << "预订结果:" << (success ? "成功" : "失败");
    if (!success) {
        qDebug() << "错误信息:" << errorMsg;
    }
}

void testAdminFunctions() {
    qDebug() << "\n===== 测试4: 管理员功能 =====";
    
    Backend& backend = Backend::instance();
    QString errorMsg;
    
    // 4.1 添加城市
    qDebug() << "\n--- 测试添加城市 ---";
    int cityId = backend.addCity("测试城市", "TEST", "中国");
    qDebug() << "添加城市，ID:" << cityId;
    
    if (cityId > 0) {
        // 验证是否添加成功
        City retrievedCity = backend.getCityById(cityId);
        qDebug() << "验证：" << retrievedCity.name() << "(" << retrievedCity.code() << ")";
    }
    
    // 4.2 添加机场
    if (cityId > 0) {
        qDebug() << "\n--- 测试添加机场 ---";
        int airportId = backend.addAirport("测试机场", "TST", cityId, 1);
        qDebug() << "添加机场，ID:" << airportId;
        
        if (airportId > 0) {
            Airport retrievedAirport = backend.getAirportById(airportId);
            qDebug() << "验证：" << retrievedAirport.name() << "(" << retrievedAirport.code() << ")";
        }
    }
    
    // 4.3 添加飞机
    qDebug() << "\n--- 测试添加飞机 ---";
    int airplaneId = backend.addAirplane("TestPlane-001", 100, 20, 10);
    qDebug() << "添加飞机，ID:" << airplaneId;
    
    if (airplaneId > 0) {
        Airplane retrievedAirplane = backend.getAirplaneById(airplaneId);
        qDebug() << "验证：" << retrievedAirplane.model();
    }
    
    // 4.4 更新航班状态（如果有航班）
    QList<FlightDetailInfo> flights = backend.getAllFlights();
    if (!flights.isEmpty()) {
        qDebug() << "\n--- 测试更新航班状态 ---";
        int testFlightId = flights.first().flightId;
        QString oldStatus = flights.first().status;
        
        qDebug() << "航班" << flights.first().flightNo << "当前状态:" << oldStatus;
        
        bool success = backend.updateFlightStatus(testFlightId, "delayed");
        if (success) {
            qDebug() << "状态已更新为: delayed";
            
            // 恢复原状态
            backend.updateFlightStatus(testFlightId, oldStatus);
            qDebug() << "状态已恢复为:" << oldStatus;
        }
    }
}

void testAddFlightAPI() {
    qDebug() << "\n===== 测试5: addFlight 字段参数接口 =====";

    Backend& backend = Backend::instance();
    QString errorMsg;

    const QString suffix = QString::number(QDateTime::currentMSecsSinceEpoch() % 1000000);

    // 1. 准备城市与机场
    const QString cityNameA = QString("AutoCityA_%1").arg(suffix);
    const QString cityCodeA = QString("ACA%1").arg(suffix.mid(0, 3));
    const QString cityNameB = QString("AutoCityB_%1").arg(suffix);
    const QString cityCodeB = QString("ACB%1").arg(suffix.mid(0, 3));

    int cityIdA = backend.addCity(cityNameA, cityCodeA, "中国");
    int cityIdB = backend.addCity(cityNameB, cityCodeB, "中国");

    qDebug() << "城市A/B ID:" << cityIdA << cityIdB;
    if (cityIdA <= 0 || cityIdB <= 0) {
        qDebug() << "添加城市失败，终止测试";
        return;
    }

    const QString airportCodeA = QString("APA%1").arg(suffix.mid(0, 3));
    const QString airportCodeB = QString("APB%1").arg(suffix.mid(0, 3));

    int airportIdA = backend.addAirport(QString("AutoAirportA_%1").arg(suffix), airportCodeA, cityIdA, 1);
    int airportIdB = backend.addAirport(QString("AutoAirportB_%1").arg(suffix), airportCodeB, cityIdB, 1);

    qDebug() << "机场A/B ID:" << airportIdA << airportIdB;
    if (airportIdA <= 0 || airportIdB <= 0) {
        qDebug() << "添加机场失败，终止测试";
        return;
    }

    // 2. 准备飞机
    const QString planeModel = QString("AutoPlane_%1").arg(suffix);
    int airplaneId = backend.addAirplane(planeModel, 60, 12, 6);
    qDebug() << "飞机 ID:" << airplaneId;
    if (airplaneId <= 0) {
        qDebug() << "添加飞机失败，终止测试";
        return;
    }

    // 3. 调用新的 addFlight 接口
    const QString flightNo = QString("AUTO%1").arg(suffix.mid(0, 4));
    QDateTime departTime = QDateTime::currentDateTime().addDays(1);
    QDateTime arriveTime = departTime.addSecs(2 * 3600);  // +2 小时

    int flightId = backend.addFlight(
        flightNo,
        airplaneId,
        airportIdA,
        airportIdB,
        departTime,
        arriveTime,
        QStringLiteral("scheduled"),
        errorMsg
    );

    if (flightId <= 0) {
        qDebug() << "添加航班失败:" << errorMsg;
        return;
    }

    qDebug() << "添加航班成功，ID:" << flightId;

    // 4. 验证航班详情与票务初始化
    FlightDetailInfo detail = backend.getFlightDetail(flightId);
    qDebug() << "航班号:" << detail.flightNo;
    qDebug() << "出发机场:" << detail.departAirportName << detail.departAirportCode;
    qDebug() << "到达机场:" << detail.arriveAirportName << detail.arriveAirportCode;
    qDebug() << "机型:" << detail.airplaneModel;

    qDebug() << "票务初始化情况:";
    for (auto it = detail.tickets.begin(); it != detail.tickets.end(); ++it) {
        const TicketInfo& ticket = it.value();
        qDebug() << "  舱位" << ticket.ticketClass
                 << "总座位:" << ticket.totalSeats
                 << "余票:" << ticket.remainSeats;
    }
}
