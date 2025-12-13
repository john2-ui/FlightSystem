#include "ui_client.h"
#include "ui_ui_client.h"
#include "../backend/backend.h"
#include "mainwindow.h"

#include <QAction>
#include <QMessageBox>
#include <QGroupBox>
#include <QTableWidgetItem>
#include <QTimer>
#include <QScreen>

ui_client::ui_client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui_client)

    , currentUserId(-1)
    , selectedFlightId(-1)
    , currentTicketId(-1)
{
    qDebug()<<"初始值:"<<currentUserId;
    ui->setupUi(this);
    this->moveToCenter();

    ui->stackedWidget->setCurrentWidget(ui->first_page);
    //初始化基础查询
    initCityQueryPage();
    initAirportQueryPage();
    initAirplaneQueryPage();

    //初始化航班查询页面
    initFlightSearchPage();
    initFlightListPage();
    initFlightDetailPage();

    ui->menu->setTitle("首页");
    ui->menu_2->setTitle("基础查询");
    ui->menu_3->setTitle("航班查询");
    ui->menu_4->setTitle("用户中心");

    connect(ui->actionfirst,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentWidget(ui->first_page);
    });

    connect(ui->search_City,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentWidget(ui->page_search_City);
    });

    connect(ui->search_Airport,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentWidget(ui->page_search_Airport);
    });

    connect(ui->search_Airplane,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentWidget(ui->page_search_Airplane);
    });

    connect(ui->actionsearchflight,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentWidget(ui->page_flight_search);
    });

    connect(ui->actiongerenzx,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentWidget(ui->page_user_center);
        loadUserTickets();
    });

    ui->radioEconomy->setChecked(true);
    resetBookingInfo();
}

// 初始化城市查询页面
void ui_client::initCityQueryPage()
{
    // 1. 设置表格样式
    ui->tableCityResult->horizontalHeader()->setStretchLastSection(false);
    ui->tableCityResult->setAlternatingRowColors(true);
    ui->tableCityResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// 按ID查询
void ui_client::on_btnQueryById_clicked()
{
    // 根据editCityId获取文本
    QString idText = ui->editCityId->text().trimmed().toUpper();
    if (idText.isEmpty()) {
        ui->labelCityStatus->setText("请输入城市ID");
        return;
    }
    bool ok;
    int id = idText.toInt(&ok);
    if (!ok || id <= 0) {
        ui->labelCityStatus->setText("请输入有效的数字ID");
        return;
    }
    City city = Backend::instance().getCityById(id);

    if (city.id() > 0) {
        QList<City> list;
        list.append(city);
        displayCities(list);
        ui->labelCityStatus->setText(QString("找到ID为 %1 的城市").arg(id));
    } else {
        ui->tableCityResult->setRowCount(0);
        ui->labelCityStatus->setText(QString("未找到ID为 %1 的城市").arg(id));
        ui->labelCityCount->setText("0 条记录");
    }
}

void ui_client::on_btnQueryByCode_clicked()
{
    QString code = ui->editCityCode->text().trimmed();

    if (code.isEmpty()) {
        ui->labelCityStatus->setText("请输入城市代码");
        return;
    }

    City city = Backend::instance().getCityByCode(code);

    if (city.id() > 0) {
        QList<City> list;
        list.append(city);
        displayCities(list);
        ui->labelCityStatus->setText(QString("找到代码为 %1 的城市").arg(code));
    } else {
        ui->tableCityResult->setRowCount(0);
        ui->labelCityStatus->setText(QString("未找到代码为 %1 的城市").arg(code));
        ui->labelCityCount->setText("0 条记录");
    }
}

// 显示所有城市
void ui_client::on_btnQueryAll_clicked()
{
    // 清空查询条件
    ui->editCityId->clear();
    ui->editCityCode->clear();
    // 获取所有城市
    currentCityList = Backend::instance().getAllCities();
    displayCities(currentCityList);
    ui->labelCityStatus->setText("显示所有城市");
}

// 搜索过滤
void ui_client::on_editSearchCity_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        displayCities(currentCityList);
        ui->labelCityStatus->setText("显示全部");
        return;
    }

    QList<City> filtered;
    QString keyword = text.toLower();

    for (const City &city : currentCityList) {
        if (city.name().toLower().contains(keyword) ||
            city.code().toLower().contains(keyword) ||
            city.country().toLower().contains(keyword)) {
            filtered.append(city);
        }
    }

    displayCities(filtered);
    ui->labelCityStatus->setText(QString("搜索: %1").arg(text));
}

// 显示城市到表格
void ui_client::displayCities(const QList<City> &cities)
{
    ui->tableCityResult->setRowCount(cities.size());

    for (int i = 0; i < cities.size(); ++i) {
        const City &city = cities[i];

        ui->tableCityResult->setItem(i, 0, new QTableWidgetItem(QString::number(city.id())));
        ui->tableCityResult->setItem(i, 1, new QTableWidgetItem(city.name()));
        ui->tableCityResult->setItem(i, 2, new QTableWidgetItem(city.code()));
        ui->tableCityResult->setItem(i, 3, new QTableWidgetItem(city.country()));
    }

    // 更新计数
    ui->labelCityCount->setText(QString("%1 条记录").arg(cities.size()));
}

// 初始化机场查询页面
void ui_client::initAirportQueryPage()
{
    // 1. 设置表格样式
    ui->tableAirportResult->horizontalHeader()->setStretchLastSection(false);
    ui->tableAirportResult->setAlternatingRowColors(true);
    ui->tableAirportResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

//机场查询方法
// 按ID查询机场
void ui_client::on_btnQueryAirportById_clicked()
{
    QString idText = ui->editAirportId->text().trimmed().toUpper();

    if (idText.isEmpty()) {
        ui->labelAirportStatus->setText("请输入机场ID");
        return;
    }

    bool ok;
    int id = idText.toInt(&ok);
    if (!ok || id <= 0) {
        ui->labelAirportStatus->setText("请输入有效的数字ID");
        return;
    }

    Airport airport = Backend::instance().getAirportById(id);

    if (airport.id() > 0) {
        QList<Airport> list;
        list.append(airport);
        displayAirports(list);
        ui->labelAirportStatus->setText(QString("找到ID为 %1 的机场").arg(id));
    } else {
        ui->tableAirportResult->setRowCount(0);
        ui->labelAirportStatus->setText(QString("未找到ID为 %1 的机场").arg(id));
        ui->labelAirportCount->setText("0 条记录");
    }
}

// 按代码查询机场
void ui_client::on_btnQueryAirportByCode_clicked()
{
    QString code = ui->editAirportCode->text().trimmed();

    if (code.isEmpty()) {
        ui->labelAirportStatus->setText("请输入机场代码");
        return;
    }

    Airport airport = Backend::instance().getAirportByCode(code);

    if (airport.id() > 0) {
        QList<Airport> list;
        list.append(airport);
        displayAirports(list);
        ui->labelAirportStatus->setText(QString("找到代码为 %1 的机场").arg(code));
    } else {
        ui->tableAirportResult->setRowCount(0);
        ui->labelAirportStatus->setText(QString("未找到代码为 %1 的机场").arg(code));
        ui->labelAirportCount->setText("0 条记录");
    }
}

// 按城市查询机场（新增）
void ui_client::on_btnQueryAirportByCity_clicked()
{
    QString cityIdText = ui->editAirportCityId->text().trimmed();

    if (cityIdText.isEmpty()) {
        ui->labelAirportStatus->setText("请输入城市ID");
        return;
    }

    bool ok;
    int cityId = cityIdText.toInt(&ok);
    if (!ok || cityId <= 0) {
        ui->labelAirportStatus->setText("请输入有效的城市ID");
        return;
    }

    // 先检查城市是否存在
    City city = Backend::instance().getCityById(cityId);
    if (city.id() <= 0) {
        ui->labelAirportStatus->setText(QString("城市ID %1 不存在").arg(cityId));
        ui->tableAirportResult->setRowCount(0);
        ui->labelAirportCount->setText("0 条记录");
        return;
    }

    QList<Airport> airports = Backend::instance().getAirportsByCity(cityId);

    if (!airports.isEmpty()) {
        displayAirports(airports);
        ui->labelAirportStatus->setText(QString("找到城市 %1 的 %2 个机场").arg(city.name()).arg(airports.size()));
    } else {
        ui->tableAirportResult->setRowCount(0);
        ui->labelAirportStatus->setText(QString("城市 %1 没有机场").arg(city.name()));
        ui->labelAirportCount->setText("0 条记录");
    }
}

// 显示所有机场
void ui_client::on_btnQueryAllAirports_clicked()
{
    // 清空查询条件
    ui->editAirportId->clear();
    ui->editAirportCode->clear();
    ui->editAirportCityId->clear();

    currentAirportList = Backend::instance().getAllAirports();
    displayAirports(currentAirportList);
    ui->labelAirportStatus->setText("显示所有机场");
}

// 机场搜索过滤
void ui_client::on_editSearchAirport_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        displayAirports(currentAirportList);
        ui->labelAirportStatus->setText("显示全部");
        return;
    }

    QList<Airport> filtered;
    QString keyword = text.toLower();

    for (int i = 0; i < currentAirportList.size(); ++i) {
        const Airport &airport = currentAirportList.at(i);
        if (airport.name().toLower().contains(keyword) ||
            airport.code().toLower().contains(keyword) ||
            QString::number(airport.cityId()).contains(keyword) ||
            QString::number(airport.terminalCount()).contains(keyword)) {
            filtered.append(airport);
        }
    }

    displayAirports(filtered);
    ui->labelAirportStatus->setText(QString("搜索: %1").arg(text));
}

// 显示机场到表格（5列）
void ui_client::displayAirports(const QList<Airport> &airports)
{
    ui->tableAirportResult->setRowCount(airports.size());

    for (int i = 0; i < airports.size(); ++i) {
        const Airport &airport = airports[i];

        ui->tableAirportResult->setItem(i, 0, new QTableWidgetItem(QString::number(airport.id())));
        ui->tableAirportResult->setItem(i, 1, new QTableWidgetItem(airport.name()));
        ui->tableAirportResult->setItem(i, 2, new QTableWidgetItem(airport.code()));
        ui->tableAirportResult->setItem(i, 3, new QTableWidgetItem(QString::number(airport.cityId())));
        ui->tableAirportResult->setItem(i, 4, new QTableWidgetItem(QString::number(airport.terminalCount())));
    }

    // 更新计数
    ui->labelAirportCount->setText(QString("%1 条记录").arg(airports.size()));
}

void ui_client::initAirplaneQueryPage()
{
    // 1. 设置表格样式
    ui->tableAirplaneResult->horizontalHeader()->setStretchLastSection(false);
    ui->tableAirplaneResult->setAlternatingRowColors(true);
    ui->tableAirplaneResult->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

// 飞机查询方法

// 按ID查询飞机
void ui_client::on_btnQueryAirplaneById_clicked()
{
    // 1. 获取输入
    QString idText = ui->editAirplaneId->text().trimmed();

    // 2. 检查输入是否为空
    if (idText.isEmpty()) {
        ui->labelAirplaneStatus->setText("请输入飞机ID");
        return;
    }

    // 3. 转换为整数
    bool ok;
    int id = idText.toInt(&ok);
    if (!ok || id <= 0) {
        ui->labelAirplaneStatus->setText("请输入有效的数字ID");
        return;
    }

    // 4. 调用后端接口查询
    Airplane airplane = Backend::instance().getAirplaneById(id);

    // 5. 处理查询结果
    if (airplane.id() > 0) {
        QList<Airplane> list;
        list.append(airplane);
        displayAirplanes(list);
        ui->labelAirplaneStatus->setText(QString("找到ID为 %1 的飞机").arg(id));
    } else {
        ui->tableAirplaneResult->setRowCount(0);
        ui->labelAirplaneStatus->setText(QString("未找到ID为 %1 的飞机").arg(id));
        ui->labelAirplaneCount->setText("0 条记录");
    }
}

// 显示所有飞机
void ui_client::on_btnQueryAllAirplanes_clicked()
{
    // 1. 清空查询条件
    ui->editAirplaneId->clear();
    //ui->editSearchAirplane->clear();

    // 2. 调用后端接口获取所有飞机
    currentAirplaneList = Backend::instance().getAllAirplanes();

    // 3. 显示结果
    displayAirplanes(currentAirplaneList);
    ui->labelAirplaneStatus->setText("显示所有飞机");
}

// 飞机搜索过滤（本地搜索）
void ui_client::on_editSearchAirplane_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        displayAirplanes(currentAirplaneList);
        ui->labelAirplaneStatus->setText("显示全部");
        return;
    }

    // 1. 创建过滤后的列表
    QList<Airplane> filtered;
    QString keyword = text.toLower();

    // 2. 遍历当前列表，筛选匹配项
    for (int i = 0; i < currentAirplaneList.size(); ++i) {
        const Airplane &airplane = currentAirplaneList.at(i);

        // 3. 检查是否匹配（支持按型号、座位数搜索）
        if (airplane.model().toLower().contains(keyword) ||
            QString::number(airplane.id()).contains(keyword) ||
            QString::number(airplane.seatsEconomy()).contains(keyword) ||
            QString::number(airplane.seatsBusiness()).contains(keyword) ||
            QString::number(airplane.seatsFirst()).contains(keyword)) {
            filtered.append(airplane);
        }
    }

    // 4. 显示过滤结果
    displayAirplanes(filtered);
    ui->labelAirplaneStatus->setText(QString("搜索: %1").arg(text));
}

// 显示飞机到表格（5列）
void ui_client::displayAirplanes(const QList<Airplane> &airplanes)
{
    // 1. 设置表格行数
    ui->tableAirplaneResult->setRowCount(airplanes.size());

    // 2. 遍历飞机列表，填充每一行
    for (int i = 0; i < airplanes.size(); ++i) {
        const Airplane &airplane = airplanes[i];

        // 3. 设置每一列的数据
        // 第1列：ID
        ui->tableAirplaneResult->setItem(i, 0, new QTableWidgetItem(QString::number(airplane.id())));
        // 第2列：飞机型号
        ui->tableAirplaneResult->setItem(i, 1, new QTableWidgetItem(airplane.model()));
        // 第3列：经济舱座位数
        ui->tableAirplaneResult->setItem(i, 2, new QTableWidgetItem(QString::number(airplane.seatsEconomy())));
        // 第4列：商务舱座位数
        ui->tableAirplaneResult->setItem(i, 3, new QTableWidgetItem(QString::number(airplane.seatsBusiness())));
        // 第5列：头等舱座位数
        ui->tableAirplaneResult->setItem(i, 4, new QTableWidgetItem(QString::number(airplane.seatsFirst())));
    }

    // 4. 更新记录计数
    ui->labelAirplaneCount->setText(QString("%1 条记录").arg(airplanes.size()));
}

// 初始化航班查询页面
void ui_client::initFlightSearchPage()
{
    // 设置日期为明天（默认查询明天的航班）
    ui->dateEditDepart->setDate(QDate::currentDate().addDays(0));

    // 加载城市数据到下拉框
    loadCitiesToComboBox();
}

// 初始化航班列表页面
void ui_client::initFlightListPage()
{
    // 确保Scroll Area可以自适应
    ui->scrollAreaFlights->setWidgetResizable(true);
}

// 初始化航班详情页面
void ui_client::initFlightDetailPage()
{
    // 设置表格样式
    ui->tableTickets->horizontalHeader()->setStretchLastSection(false);
    ui->tableTickets->setAlternatingRowColors(true);
    ui->tableTickets->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

// 加载城市到下拉框
void ui_client::loadCitiesToComboBox()
{
    ui->comboFromCity->clear();
    ui->comboToCity->clear();

    // 获取所有城市
    QList<City> cities = Backend::instance().getAllCities();

    // 按城市名称排序
    std::sort(cities.begin(), cities.end(),
              [](const City& a, const City& b) { return a.name() < b.name(); });

    // 添加到下拉框，格式：北京 (BJS)
    foreach (const City& city, cities) {
        QString displayText = QString("%1 (%2)").arg(city.name()).arg(city.code());
        QString cityCode = city.code();

        ui->comboFromCity->addItem(displayText, cityCode);
        ui->comboToCity->addItem(displayText, cityCode);
    }

    // 设置默认值（北京->上海）
    int beijingIndex = ui->comboFromCity->findText("北京 (BJS)", Qt::MatchContains);
    int shanghaiIndex = ui->comboToCity->findText("上海 (SHA)", Qt::MatchContains);

    if (beijingIndex != -1) ui->comboFromCity->setCurrentIndex(beijingIndex);
    if (shanghaiIndex != -1) ui->comboToCity->setCurrentIndex(shanghaiIndex);
}

// 菜单：航班查询
void ui_client::on_actionSearchFlight_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->page_flight_search);
}

// 交换城市按钮
void ui_client::on_btnSwapCities_clicked()
{
    // 获取当前选择
    int fromIndex = ui->comboFromCity->currentIndex();
    int toIndex = ui->comboToCity->currentIndex();

    // 交换选择
    if (fromIndex >= 0 && toIndex >= 0) {
        QString fromText = ui->comboFromCity->currentText();
        QVariant fromData = ui->comboFromCity->itemData(fromIndex);

        QString toText = ui->comboToCity->currentText();
        QVariant toData = ui->comboToCity->itemData(toIndex);

        // 交换
        ui->comboFromCity->setCurrentIndex(toIndex);
        ui->comboToCity->setCurrentIndex(fromIndex);
    }
}

// 查询按钮
void ui_client::on_btnSearch_clicked()
{
    // 1. 获取查询条件
    int fromIndex = ui->comboFromCity->currentIndex();
    int toIndex = ui->comboToCity->currentIndex();

    if (fromIndex < 0 || toIndex < 0) {
        QMessageBox::warning(this, "提示", "请选择出发和到达城市");
        return;
    }

    // 获取城市代码（从关联数据）
    currentFromCityCode = ui->comboFromCity->itemData(fromIndex).toString();
    currentToCityCode = ui->comboToCity->itemData(toIndex).toString();
    currentDepartDate = ui->dateEditDepart->date();

    // 2. 验证输入
    if (currentFromCityCode.isEmpty() || currentToCityCode.isEmpty()) {
        QMessageBox::warning(this, "提示", "城市选择无效");
        return;
    }

    if (currentFromCityCode == currentToCityCode) {
        QMessageBox::warning(this, "提示", "出发和到达城市不能相同");
        return;
    }

    if (currentDepartDate < QDate::currentDate()) {
        QMessageBox::warning(this, "提示", "出发日期不能是过去");
        return;
    }

    // 3. 调用后端接口查询航班
    currentFlightList = Backend::instance().searchFlights(
        currentFromCityCode,
        currentToCityCode,
        currentDepartDate
        );

    // 4. 显示结果到列表页面
    showFlightListPage();
}

// 显示航班列表页面
void ui_client::showFlightListPage()
{
    // 更新页面标题和查询信息
    QString fromCityName = ui->comboFromCity->currentText();
    QString toCityName = ui->comboToCity->currentText();

    ui->labelRouteInfo->setText(QString("%1 → %2").arg(fromCityName).arg(toCityName));
    ui->labelDateInfo->setText(currentDepartDate.toString("yyyy-MM-dd"));
    ui->labelCountInfo->setText(QString("共找到 %1 个航班").arg(currentFlightList.size()));

    // 显示航班卡片
    displayFlightCards();

    // 切换到列表页面
    ui->stackedWidget->setCurrentWidget(ui->page_flight_list);
}

// 返回按钮（列表页面返回到查询页面）
void ui_client::on_btnBackFromList_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_flight_search);
}

// 显示航班卡片
void ui_client::displayFlightCards()
{
    // 清空现有的卡片
    clearFlightCards();

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setObjectName("layoutCards");
    }

    int normalFlightCount = 0; // 统计正常航班数量

    // 为每个航班创建卡片，只显示status为normal的航班
    for (const FlightDetailInfo& flight : currentFlightList) {
        // 检查航班状态，只显示normal状态的航班
        if (flight.status == "normal") {
            QGroupBox* card = createFlightCard(flight);
            layout->addWidget(card);
            normalFlightCount++;
        }
    }

    // 更新航班数量显示
    ui->labelCountInfo->setText(QString("共找到 %1 个航班").arg(normalFlightCount));

    // 如果没有正常航班，显示提示信息
    if (normalFlightCount == 0) {
        QLabel* lblNoFlight = new QLabel("没有找到可用的航班");
        lblNoFlight->setAlignment(Qt::AlignCenter);
        lblNoFlight->setStyleSheet("color: #999; font-size: 16px; padding: 40px;");
        layout->addWidget(lblNoFlight);
    }

    // 添加一个拉伸，让卡片靠上显示
    layout->addStretch();
}

// 创建单个航班卡片
QGroupBox* ui_client::createFlightCard(const FlightDetailInfo& flight)
{
    QGroupBox* card = new QGroupBox();
    card->setMinimumHeight(140);
    card->setMinimumWidth(300);
    card->setStyleSheet(
        "QGroupBox {"
        "  border: 1px solid #ddd;"
        "  border-radius: 8px;"
        "  margin: 5px;"
        "  padding: 10px;"
        "  background-color: white;"
        "}"
        "QGroupBox:hover {"
        "  border-color: #0066CC;"
        "  background-color: #f8f9fa;"
        "}"
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(card);

    // 第一行：航班号和时间
    QHBoxLayout* row1 = new QHBoxLayout();
    //row1->setSpacing(15);
    //row1->setContentsMargins(0,0,0,8);

    // 航班号（蓝色粗体）
    QLabel* lblFlightNo = new QLabel(QString("✈️ <b style='color:#0066CC; font-size:16px;'>%1</b>").arg(flight.flightNo));
    row1->addWidget(lblFlightNo);

    // 时间信息
    QString timeInfo = QString("%1 — %2  (%3)")
                           .arg(flight.departTime.toString("hh:mm"))
                           .arg(flight.arriveTime.toString("hh:mm"))
                           .arg(formatDuration(flight.departTime, flight.arriveTime));

    QLabel* lblTime = new QLabel(timeInfo);
    lblTime->setStyleSheet("color: #666; font-size: 14px;");
    row1->addWidget(lblTime);
    row1->addStretch();

    mainLayout->addLayout(row1);

    // 第二行：机场路线
    QHBoxLayout* row2 = new QHBoxLayout();

    QString airportInfo = QString("%1 → %2")
                              .arg(flight.departAirportName)
                              .arg(flight.arriveAirportName);

    QLabel* lblAirports = new QLabel(airportInfo);
    lblAirports->setStyleSheet("font-size: 14px;");
    row2->addWidget(lblAirports);
    row2->addStretch();

    mainLayout->addLayout(row2);

    // 第三行：票价和选择按钮
    QHBoxLayout* row3 = new QHBoxLayout();

    // 显示最低票价
    double minPrice = 999999;
    QString minPriceClass;
    for (auto it = flight.tickets.begin(); it != flight.tickets.end(); ++it) {
        if (it.value().price < minPrice && it.value().remainSeats > 0) {
            minPrice = it.value().price;
            minPriceClass = it.key();
        }
    }

    if (minPrice < 999999) {
        QLabel* lblPrice = new QLabel(QString("<b style='color:#FF6600; font-size:16px;'>¥%1</b> 起").arg(minPrice));
        row3->addWidget(lblPrice);
    } else {
        QLabel* lblPrice = new QLabel("<span style='color:#999;'>已售罄</span>");
        row3->addWidget(lblPrice);
    }

    row3->addStretch();

    // 选择按钮
    QPushButton* btnSelect = new QPushButton("选择");
    btnSelect->setProperty("flightId", flight.flightId);
    btnSelect->setStyleSheet(
        "background-color: #FF6600;"
        "color: white;"
        "padding: 8px 20px;"
        "border: none;"
        "border-radius: 4px;"
        "font-size: 16px;"
        );

    connect(btnSelect, &QPushButton::clicked, [this, flight]() {
        displayFlightDetail(flight.flightId);
    });

    row3->addWidget(btnSelect);
    mainLayout->addLayout(row3);

    return card;
}

// 清空航班卡片
void ui_client::clearFlightCards()
{
    QLayout* layout = ui->scrollAreaWidgetContents->layout();
    if (!layout) return;

    // 删除所有子部件
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

// 返回按钮（详情页面返回到列表页面）
void ui_client::on_btnBackFromDetail_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_flight_list);
}

// 预订按钮
void ui_client::on_btnBook_clicked()
{
    if (selectedFlightId > 0) {
        QMessageBox::information(this, "提示",
                                 QString("即将预订航班 %1，预订功能将在机票预订模块实现").arg(selectedFlightId));
    }
}

// 显示航班详情
void ui_client::displayFlightDetail(int flightId)
{
    // 保存选中的航班ID
    selectedFlightId = flightId;

    resetBookingInfo();

    // 调用后端接口获取航班详情
    FlightDetailInfo flight = Backend::instance().getFlightDetail(flightId);

    if (flight.flightId == 0) {
        QMessageBox::warning(this, "错误", "航班不存在或已取消");
        return;
    }

    // 更新基本信息
    ui->labelFlightNo->setText(flight.flightNo);
    ui->labelStatus->setText(flight.status);

    // 设置状态颜色
    QString statusColor = "#4CAF50"; // 绿色
    if (flight.status.contains("延误")) statusColor = "#FF9800"; // 橙色
    if (flight.status.contains("取消")) statusColor = "#F44336"; // 红色
    ui->labelStatus->setStyleSheet(QString("color: %1; font-weight: bold;").arg(statusColor));

    // 出发信息
    ui->labelDepartAirport->setText(
        QString("%1 (%2)").arg(flight.departAirportName).arg(flight.departAirportCode));
    ui->labelDepartTime->setText(flight.departTime.toString("yyyy-MM-dd hh:mm"));

    // 到达信息
    ui->labelArriveAirport->setText(
        QString("%1 (%2)").arg(flight.arriveAirportName).arg(flight.arriveAirportCode));
    ui->labelArriveTime->setText(flight.arriveTime.toString("yyyy-MM-dd hh:mm"));

    // 飞行信息
    ui->labelDuration->setText(formatDuration(flight.departTime, flight.arriveTime));
    QString model=flight.airplaneModel.isEmpty()?
                   "未知型号":
                   QString("🛩 %1").arg(flight.airplaneModel);
    ui->labelAirplaneModel->setText(model);

    // 填充机票表格
    fillTicketTable(flight.tickets);

    // 更新舱位选择UI
    updateSeatSelectionUI();

    // 默认选中经济舱
    ui->radioEconomy->setChecked(true);
    on_radioEconomy_clicked();

    // 切换到详情页面
    ui->stackedWidget->setCurrentWidget(ui->page_flight_detail);
}

// 填充机票表格
void ui_client::fillTicketTable(const QMap<QString, TicketInfo>& tickets)
{
    ui->tableTickets->setRowCount(tickets.size());

    int row = 0;
    for (auto it = tickets.begin(); it != tickets.end(); ++it) {
        const TicketInfo& ticket = it.value();

        // 舱位等级
        QTableWidgetItem* itemClass = new QTableWidgetItem(ticket.ticketClass);
        itemClass->setTextAlignment(Qt::AlignCenter);
        ui->tableTickets->setItem(row, 0, itemClass);

        // 总座位数
        QTableWidgetItem* itemTotal = new QTableWidgetItem(QString::number(ticket.totalSeats));
        itemTotal->setTextAlignment(Qt::AlignCenter);
        ui->tableTickets->setItem(row, 1, itemTotal);

        // 剩余座位数
        QTableWidgetItem* itemRemain = new QTableWidgetItem(QString::number(ticket.remainSeats));
        itemRemain->setTextAlignment(Qt::AlignCenter);

        // 根据余票数量设置颜色
        if (ticket.remainSeats == 0) {
            itemRemain->setForeground(Qt::red);
            itemRemain->setText("已售罄");
        } else if (ticket.remainSeats < 10) {
            itemRemain->setForeground(QColor(255, 102, 0)); // 橙色
        } else {
            itemRemain->setForeground(QColor(0, 153, 0)); // 绿色
        }

        ui->tableTickets->setItem(row, 2, itemRemain);

        // 价格
        QTableWidgetItem* itemPrice = new QTableWidgetItem(QString("¥%1").arg(ticket.price));
        itemPrice->setTextAlignment(Qt::AlignCenter);
        ui->tableTickets->setItem(row, 3, itemPrice);

        row++;
    }
}

// 格式化飞行时长
QString ui_client::formatDuration(const QDateTime& start, const QDateTime& end)
{
    qint64 seconds = start.secsTo(end);
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;

    if (hours > 0 && minutes > 0) {
        return QString("%1小时%2分钟").arg(hours).arg(minutes);
    } else if (hours > 0) {
        return QString("%1小时").arg(hours);
    } else {
        return QString("%1分钟").arg(minutes);
    }
}

// ==================== 用户中心相关方法 ====================

// 设置用户ID
void ui_client::setCurrentUserId(int userId)
{
    currentUserId = userId;
    if(userId>0){
        loadUserInfo();
    }

}

// 设置用户名
void ui_client::setCurrentUsername(const QString &username)
{
    currentUsername = username;
    updateUserInfoDisplay();
}

// 加载用户信息
void ui_client::loadUserInfo()
{
    if (currentUserId <= 0) return;

    // 从数据库获取用户信息
    User user = Backend::instance().getUserById(currentUserId);

    if (user.id() > 0) {
        currentUsername = user.username();
        updateUserInfoDisplay();
    }
}

// 更新用户信息显示
void ui_client::updateUserInfoDisplay()
{
    if (currentUserId > 0) {
        ui->labelUserName->setText(currentUsername);
        // 从数据库获取完整用户信息以显示类型
        User user = Backend::instance().getUserById(currentUserId);
        ui->labelUserPassword->setText(user.password());
    } else {
        ui->labelUserName->setText("（未登录）");
    }
}

// 菜单：个人中心
void ui_client::on_actionUserCenter_triggered()
{
    if (currentUserId <= 0) {
        QMessageBox::warning(this, "提示", "请先登录");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->page_user_center);
    loadUserInfo();
}

void ui_client::updateSeatSelectionUI()
{
    // 获取机票信息
    FlightDetailInfo flight = Backend::instance().getFlightDetail(selectedFlightId);
    if (flight.flightId == 0) return;

    const QMap<QString, TicketInfo>& tickets = flight.tickets;

    // 设置单选按钮是否可用（根据余票）
    if (tickets.contains("economy")) {
        ui->radioEconomy->setEnabled(tickets["economy"].remainSeats > 0);
        ui->radioEconomy->setToolTip(tickets["economy"].remainSeats > 0
                                         ? QString("余票: %1张").arg(tickets["economy"].remainSeats)
                                         : "已售罄");
    }

    if (tickets.contains("business")) {
        ui->radioBusiness->setEnabled(tickets["business"].remainSeats > 0);
        ui->radioBusiness->setToolTip(tickets["business"].remainSeats > 0
                                          ? QString("余票: %1张").arg(tickets["business"].remainSeats)
                                          : "已售罄");
    }

    if (tickets.contains("first")) {
        ui->radioFirst->setEnabled(tickets["first"].remainSeats > 0);
        ui->radioFirst->setToolTip(tickets["first"].remainSeats > 0
                                       ? QString("余票: %1张").arg(tickets["first"].remainSeats)
                                       : "已售罄");
    }
    // 如果没有可用舱位，禁用订票按钮
    bool hasAvailable = false;
    for (const TicketInfo& ticket : tickets.values()) {
        if (ticket.remainSeats > 0) {
            hasAvailable = true;
            break;
        }
    }
    ui->btnConfirmBook->setEnabled(hasAvailable);
}

void ui_client::on_radioEconomy_clicked()
{
    if (!ui->radioEconomy->isEnabled()) return;

    FlightDetailInfo flight = Backend::instance().getFlightDetail(selectedFlightId);
    if (!flight.tickets.contains("economy")) return;

    const TicketInfo& ticket = flight.tickets["economy"];
    currentTicketClass = "economy";
    currentTicketPrice = ticket.price;
    currentTicketId = ticket.ticketId;
    ui->btnConfirmBook->setText(QString("确认订票 (¥%1)").arg(currentTicketPrice));
}

void ui_client::on_radioBusiness_clicked()
{
    if (!ui->radioBusiness->isEnabled()) return;

    FlightDetailInfo flight = Backend::instance().getFlightDetail(selectedFlightId);
    if (!flight.tickets.contains("business")) return;

    const TicketInfo& ticket = flight.tickets["business"];
    currentTicketClass = "business";
    currentTicketPrice = ticket.price;
    currentTicketId = ticket.ticketId;

    ui->btnConfirmBook->setText(QString("确认订票 (¥%1)").arg(currentTicketPrice));
}

void ui_client::on_radioFirst_clicked()
{
    if (!ui->radioFirst->isEnabled()) return;

    FlightDetailInfo flight = Backend::instance().getFlightDetail(selectedFlightId);
    if (!flight.tickets.contains("first")) return;

    const TicketInfo& ticket = flight.tickets["first"];
    currentTicketClass = "first";
    currentTicketPrice = ticket.price;
    currentTicketId = ticket.ticketId;

    ui->btnConfirmBook->setText(QString("确认订票 (¥%1)").arg(currentTicketPrice));
}

void ui_client::on_btnConfirmBook_clicked()
{
    qDebug()<<"当前值:"<<currentUserId<<"票ID"<<currentTicketId;
    // 验证用户是否登录
    if (currentUserId <= 0) {
        QMessageBox::warning(this, "请先登录", "您需要先登录才能订票");
        return;
    }

    // 验证订票信息
    if (selectedFlightId <= 0 || currentTicketId <= 0 || currentTicketClass.isEmpty()) {
        QMessageBox::warning(this, "错误", "请选择舱位");
        return;
    }

    // 获取最新余票信息
    FlightDetailInfo flight = Backend::instance().getFlightDetail(selectedFlightId);
    if (!flight.tickets.contains(currentTicketClass)) {
        QMessageBox::warning(this, "错误", "该舱位不存在");
        return;
    }

    int remainSeats = flight.tickets[currentTicketClass].remainSeats;
    if (remainSeats <= 0) {
        QMessageBox::warning(this, "抱歉", "该舱位已售罄，请选择其他舱位");
        updateSeatSelectionUI();
        return;
    }

    // 确认对话框
    /*QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认订票",
                                  QString("您确定要预订：\n"
                                          "航班：%1\n"
                                          "舱位：%2\n"
                                          "价格：¥%3\n\n"
                                          "确认支付吗？")
                                      .arg(ui->labelFlightNo->text())
                                      .arg(currentTicketClass)
                                      .arg(currentTicketPrice),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }*/

    // 确认对话框（修改按钮文本）
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("确认订票");
    msgBox.setText(QString("您确定要预订：\n"
                           "航班：%1\n"
                           "舱位：%2\n"
                           "价格：¥%3\n\n"
                           "确认支付吗？")
                       .arg(ui->labelFlightNo->text())
                       .arg(currentTicketClass)
                       .arg(currentTicketPrice));
    msgBox.setIcon(QMessageBox::Question);

    // 设置标准按钮
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    // 修改按钮文本为中文
    msgBox.button(QMessageBox::Yes)->setText("确认");
    msgBox.button(QMessageBox::No)->setText("取消");

    int reply = msgBox.exec();

    if (reply != QMessageBox::Yes) {
        return;
    }


    // 调用后端购票接口
    QString errorMsg;
    bool success = Backend::instance().purchaseTicket(currentUserId,
                                                      currentTicketId,
                                                      1,  // 数量：1张
                                                      errorMsg);

    if (success) {
        QMessageBox::information(this, "订票成功",
                                 QString("订票成功！\n"
                                         "订单已保存到您的账户\n"
                                         "航班：%1\n"
                                         "舱位：%2\n"
                                         "价格：¥%3")
                                     .arg(ui->labelFlightNo->text())
                                     .arg(currentTicketClass)
                                     .arg(currentTicketPrice));

        // 刷新显示
        displayFlightDetail(selectedFlightId);

        // 更新用户中心信息
        loadUserInfo();

    } else {
        QMessageBox::critical(this, "订票失败",
                              QString("订票失败：%1").arg(errorMsg));
    }
}

void ui_client::resetBookingInfo()
{
    currentTicketId = -1;
    currentTicketClass.clear();
    currentTicketPrice = 0.0;
    ui->btnConfirmBook->setText("确认订票");
    ui->btnConfirmBook->setEnabled(false);
}

bool ui_client::validateBooking()
{
    if (currentUserId <= 0) {
        QMessageBox::warning(this, "请先登录", "您需要先登录才能订票");
        return false;
    }

    if (selectedFlightId <= 0) {
        QMessageBox::warning(this, "错误", "请选择航班");
        return false;
    }

    if (currentTicketId <= 0 || currentTicketClass.isEmpty()) {
        QMessageBox::warning(this, "错误", "请选择舱位");
        return false;
    }

    return true;
}

void ui_client::loadUserTickets()
{
    if (currentUserId <= 0) {
        QMessageBox::warning(this, "提示", "请先登录");
        return;
    }

    // 清空现有卡片
    clearTicketCards();

    // 更新状态
    ui->labelTicketStatus->setText("正在查询数据库...");
    ui->labelTicketStatus->setStyleSheet("color: #2196F3;");
    QApplication::processEvents();  // 更新UI显示

    // 获取用户信息
    User user = Backend::instance().getUserById(currentUserId);
    if (user.id() <= 0) {
        showNoTicketMessage("获取用户信息失败");
        ui->labelTicketStatus->setText("获取用户信息失败");
        ui->labelTicketStatus->setStyleSheet("color: #F44336;");
        return;
    }

    // 获取用户的所有票ID
    QVector<int> ticketIds = user.ticketsID();

    if (ticketIds.isEmpty()) {
        showNoTicketMessage("暂无机票记录，快去预订吧！");
        // 清空统计信息
        ui->labelTotalTickets->setText("总票数：0");
        ui->labelTotalAmount->setText("总金额：¥0");
        ui->labelStatusSummary->setText("状态：无");
        ui->labelTicketStatus->setText("无机票记录");
        ui->labelTicketStatus->setStyleSheet("color: #9E9E9E;");
        return;
    }

    qDebug() << "用户" << currentUserId << "有" << ticketIds.size() << "张票";

    // 存储所有票和航班信息的临时列表，用于排序
    QList<QPair<Ticket, FlightDetailInfo>> ticketFlights;

    // 获取所有票和航班信息
    for (int ticketId : ticketIds) {
        // 获取票信息
        Ticket ticket = Backend::instance().getTicketById(ticketId);
        if (ticket.id() <= 0) {
            qDebug() << "获取票信息失败，票ID:" << ticketId;
            continue;
        }

        // 获取航班详情
        FlightDetailInfo flight = Backend::instance().getFlightDetail(ticket.flightId());
        if (flight.flightId <= 0) {
            qDebug() << "获取航班详情失败，航班ID:" << ticket.flightId();
            continue;
        }

        ticketFlights.append(qMakePair(ticket, flight));
    }

    if (ticketFlights.isEmpty()) {
        showNoTicketMessage("未找到有效机票");
        // 清空统计信息
        ui->labelTotalTickets->setText("总票数：0");
        ui->labelTotalAmount->setText("总金额：¥0");
        ui->labelStatusSummary->setText("状态：无");
        ui->labelTicketStatus->setText("未找到有效机票");
        ui->labelTicketStatus->setStyleSheet("color: #F44336;");
        return;
    }

    qDebug() << "成功获取" << ticketFlights.size() << "张有效机票";

    // 按起飞时间排序（最近的在前）
    std::sort(ticketFlights.begin(), ticketFlights.end(),
              [](const QPair<Ticket, FlightDetailInfo>& a,
                 const QPair<Ticket, FlightDetailInfo>& b) {
                  return a.second.departTime < b.second.departTime;
              });

    // 创建或获取布局
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents_2->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->scrollAreaWidgetContents_2);
        layout->setSpacing(8);
    }

    // 统计变量
    int totalTickets = ticketFlights.size();
    double totalAmount = 0.0;
    int normalCount = 0;
    int delayedCount = 0;
    int cancelledCount = 0;

    // 创建并添加卡片
    for (const auto& pair : ticketFlights) {
        const Ticket& ticket = pair.first;
        const FlightDetailInfo& flight = pair.second;

        // 创建卡片
        QGroupBox* card = createTicketCard(ticket, flight);
        if (card) {
            layout->addWidget(card);

            // 统计
            totalAmount += ticket.price();

            QString status = flight.status.toLower();
            if (status == "normal") {
                normalCount++;
            } else if (status == "delayed") {
                delayedCount++;
            } else if (status == "cancelled" || status == "canceled") {
                cancelledCount++;
            }
        }
    }

    // 添加拉伸，让卡片靠上显示
    layout->addStretch();

    // 更新统计信息
    ui->labelTotalTickets->setText(QString("总票数：%1").arg(totalTickets));
    ui->labelTotalAmount->setText(QString("总金额：¥%1").arg(totalAmount, 0, 'f', 2));

    // 状态统计
    QString statusText;
    if (totalTickets == 0) {
        statusText = "状态：无";
    } else if (delayedCount == 0 && cancelledCount == 0) {
        statusText = "状态：全部正常";
    } else {
        statusText = QString("状态：正常%1，延误%2，取消%3")
                         .arg(normalCount)
                         .arg(delayedCount)
                         .arg(cancelledCount);
    }
    ui->labelStatusSummary->setText(statusText);

    // 更新状态栏
    ui->labelTicketStatus->setText(QString("已加载 %1 张机票").arg(totalTickets));
    ui->labelTicketStatus->setStyleSheet("color: #4CAF50;");

    // 3秒后清空状态
    QTimer::singleShot(3000, [this]() {
        ui->labelTicketStatus->clear();
    });

    // 调试信息
    qDebug() << "统计信息："
             << "总票数：" << totalTickets
             << "总金额：" << totalAmount
             << "正常：" << normalCount
             << "延误：" << delayedCount
             << "取消：" << cancelledCount;
}

// 修改函数签名，直接接收 Ticket 和 FlightDetailInfo
QGroupBox* ui_client::createTicketCard(const Ticket& ticket, const FlightDetailInfo& flight)
{
    // 创建卡片容器
    QGroupBox* card = new QGroupBox();
    card->setProperty("ticketId", ticket.id());
    card->setMinimumHeight(250);

    // 状态颜色
    QString statusColor = getStatusColor(flight.status);
    QString statusText = translateStatus(flight.status);

    // 卡片样式（和之前一样）
    card->setStyleSheet(
        QString(
            "QGroupBox {"
            "  border: 1px solid #e0e0e0;"
            "  border-radius: 8px;"
            "  margin: 10px 5px;"
            "  padding: 12px;"
            "  background: white;"
            "}"
            "QGroupBox:hover {"
            "  border-color: #bbdefb;"
            "  background: #f8f9fa;"
            "}"
            ).arg(statusColor)
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(card);
    mainLayout->setSpacing(6);

    // === 第一行：航班号和状态 ===
    QHBoxLayout* row1 = new QHBoxLayout();

    // 航班号
    QLabel* lblFlightNo = new QLabel(QString("✈️ %1").arg(flight.flightNo));
    lblFlightNo->setStyleSheet("font-weight: bold; color: #1565C0; font-size: 15px;");
    row1->addWidget(lblFlightNo);

    // 状态
    QLabel* lblStatus = new QLabel(statusText);
    lblStatus->setStyleSheet(
        QString(
            "color: %1;"
            "font-weight: bold;"
            "padding: 2px 10px;"
            "background: %2;"
            "border-radius: 10px;"
            "font-size: 11px;"
            "border: 1px solid %1;"
            ).arg(statusColor).arg(QColor(statusColor).lighter(150).name())
        );
    row1->addWidget(lblStatus);

    row1->addStretch();
    mainLayout->addLayout(row1);

    // === 第二行：机场信息 ===
    // 使用 flight 中的机场信息
    QString airportText = QString("%1 → %2")
                              .arg(flight.departAirportName)
                              .arg(flight.arriveAirportName);

    QLabel* lblAirport = new QLabel(airportText);
    lblAirport->setStyleSheet("font-weight: bold; color: #0D47A1; font-size: 13px;");
    mainLayout->addWidget(lblAirport);

    // === 第三行：时间、日期、机型 ===
    QHBoxLayout* row3 = new QHBoxLayout();

    // 时间
    QString timeText = QString("%1 %2 —— %3 %4")
                           .arg(flight.departTime.toString("yyyy-MM-dd"))
                           .arg(flight.departTime.toString("hh:mm"))
                           .arg(flight.arriveTime.toString("yyyy-MM-dd"))
                           .arg(flight.arriveTime.toString("hh:mm"));
    QLabel* lblTime = new QLabel(timeText);
    lblTime->setStyleSheet("color: #37474F; font-size: 12px;");
    row3->addWidget(lblTime);

    // 分隔符
    QLabel* dot1 = new QLabel("·");
    dot1->setStyleSheet("color: #BDBDBD; margin: 0 8px;");
    row3->addWidget(dot1);

    // 飞机机型（从 flight 获取）
    if (!flight.airplaneModel.isEmpty()) {
        QLabel* lblModel = new QLabel(QString("🛩 %1").arg(flight.airplaneModel));
        lblModel->setStyleSheet("color: #546E7A; font-size: 11px;");
        row3->addWidget(lblModel);
    }

    row3->addStretch();
    mainLayout->addLayout(row3);

    // === 第四行：舱位和价格 ===
    QHBoxLayout* row4 = new QHBoxLayout();

    // 舱位（从 ticket 获取）
    QString classText = ticket.tClass();
    if (classText == "economy") classText = "经济舱";
    else if (classText == "business") classText = "商务舱";
    else if (classText == "first") classText = "头等舱";

    QLabel* lblClass = new QLabel(classText);
    lblClass->setStyleSheet("color: #5D4037; font-size: 12px; font-weight: bold;");
    row4->addWidget(lblClass);

    row4->addStretch();

    // 价格（从 ticket 获取）
    QLabel* lblPrice = new QLabel(QString("¥%1").arg(ticket.price(), 0, 'f', 0));
    lblPrice->setStyleSheet("color: #FF6F00; font-size: 14px; font-weight: bold;");
    row4->addWidget(lblPrice);

    mainLayout->addLayout(row4);

    // === 第五行：退票按钮 ===
    QHBoxLayout* row5 = new QHBoxLayout();
    row5->addStretch();

    QPushButton* btnRefund = new QPushButton();
    btnRefund->setProperty("ticketId", ticket.id());

    if (canRefundTicket(flight.status)) {
        btnRefund->setText("退票");
        btnRefund->setEnabled(true);
        btnRefund->setStyleSheet(
            "background-color: #FF5252;"
            "color: white;"
            "padding: 5px 20px;"
            "border: none;"
            "border-radius: 4px;"
            "font-size: 12px;"
            "font-weight: bold;"
            );

        connect(btnRefund, &QPushButton::clicked, [this, ticket]() {
            onRefundButtonClicked(ticket.id());
        });
    } else {
        btnRefund->setText("已自动退票");
        btnRefund->setEnabled(false);
        btnRefund->setStyleSheet(
            "background-color: #E0E0E0;"
            "color: #9E9E9E;"
            "padding: 5px 20px;"
            "border: none;"
            "border-radius: 4px;"
            "font-size: 12px;"
            "font-weight: bold;"
            );
    }

    row5->addWidget(btnRefund);
    mainLayout->addLayout(row5);

    return card;
}

// 翻译状态
QString ui_client::translateStatus(const QString& status)
{
    if (status.toLower() == "normal") return "正常";
    if (status.toLower() == "delayed") return "延误";
    if (status.toLower() == "cancelled" || status.toLower() == "canceled")
        return "取消";
    return status;
}

// 获取状态颜色
QString ui_client::getStatusColor(const QString& status)
{
    QString lowerStatus = status.toLower();
    if (lowerStatus == "normal") return "#4CAF50";    // 绿色
    if (lowerStatus == "delayed") return "#FF9800";   // 橙色
    if (lowerStatus == "cancelled" || lowerStatus == "canceled")
        return "#F44336";  // 红色
    return "#757575";  // 灰色
}

// 判断是否可以退票
bool ui_client::canRefundTicket(const QString& status)
{
    QString lowerStatus = status.toLower();
    // normal和delayed状态可以退票，cancelled状态不可退
    return (lowerStatus == "normal" || lowerStatus == "delayed");
}

// 清空卡片
void ui_client::clearTicketCards()
{
    QLayout* layout = ui->scrollAreaWidgetContents_2->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->scrollAreaWidgetContents_2);
        return;
    }

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

// 显示无票信息
void ui_client::showNoTicketMessage(const QString& message)
{
    clearTicketCards();

    QLabel* emptyLabel = new QLabel(message);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #757575;"
        "padding: 40px 20px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents_2);
    layout->addStretch();
    layout->addWidget(emptyLabel);
    layout->addStretch();
}

// 刷新按钮
void ui_client::on_btnRefreshTickets_clicked()
{
    loadUserTickets();
    ui->labelTicketStatus->setText("已刷新");
    ui->labelTicketStatus->setStyleSheet("color: green;");

    // 2秒后清空状态
    QTimer::singleShot(2000, [this]() {
        ui->labelTicketStatus->clear();
    });
}

void ui_client::onRefundButtonClicked(int ticketId)
{
    // 1. 获取票信息（只需要价格）
    Ticket ticket = Backend::instance().getTicketById(ticketId);
    if (ticket.id() <= 0) {
        QMessageBox::warning(this, "错误", "未找到票信息");
        return;
    }

    // 2. 获取航班信息（只需要状态）
    FlightDetailInfo flight = Backend::instance().getFlightDetail(ticket.flightId());
    if (flight.flightId <= 0) {
        QMessageBox::warning(this, "错误", "未找到航班信息");
        return;
    }

    // 3. 检查是否可退票
    if (!canRefundTicket(flight.status)) {
        QString statusText = translateStatus(flight.status);
        QMessageBox::warning(this, "无法退票",
                             QString("当前航班状态为【%1】，不可退票").arg(statusText));
        return;
    }
    // 4. 修改确认对话框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("退票确认");
    msgBox.setText(QString("确定要退票吗？\n"
                           "金额：¥%1")
                       .arg(ticket.price(), 0, 'f', 2));
    msgBox.setIcon(QMessageBox::Question);

    // 设置标准按钮
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);  // 默认选择"否"，防止误操作

    // 修改按钮文本为中文
    msgBox.button(QMessageBox::Yes)->setText("确认退票");
    msgBox.button(QMessageBox::No)->setText("我再想想");

    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        // 5. 调用后端退票接口
        QString errorMsg;
        bool success = Backend::instance().refundTicket(
            currentUserId, ticketId, 1, errorMsg);

        if (success) {
            QMessageBox::information(this, "退票成功", "退票成功！");
            loadUserTickets();  // 刷新列表
        } else {
            QMessageBox::critical(this, "退票失败", errorMsg);
        }
    }
}

ui_client::~ui_client()
{
    delete ui;
}

void ui_client::moveToCenter()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    QRect screenGeometry = screen->availableGeometry();
    QSize windowSize = this->size();

    int x = (screenGeometry.width() - windowSize.width()) / 2;
    int y = (screenGeometry.height() - windowSize.height()) / 2;
    x = qMax(screenGeometry.left(), x);
    y = qMax(screenGeometry.top(), y);

    this->move(x, y);
}

void ui_client::on_btnDeleteAccount_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("确认注销账号");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("⚠️ 您确定要注销账号吗？");
    msgBox.setInformativeText("此操作不可撤销！所有数据将被永久删除。");

    // 自定义按钮
    QPushButton* deleteButton = msgBox.addButton("确认注销", QMessageBox::AcceptRole);
    QPushButton* cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);

    // 设置按钮样式
    deleteButton->setStyleSheet(
        "background-color: #F44336;"
        "color: white;"
        "padding: 8px 20px;"
        "border-radius: 4px;"
        "font-weight: bold;"
        );
    cancelButton->setStyleSheet(
        "background-color: #E0E0E0;"
        "color: #333;"
        "padding: 8px 20px;"
        "border-radius: 4px;"
        );

    msgBox.exec();

    if (msgBox.clickedButton() == deleteButton) {
        // 执行注销
        QString errorMsg;
        bool success = Backend::instance().deleteUser(currentUserId, errorMsg);

        if (success) {
            QMessageBox::information(this, "注销成功", "账号已成功注销");
            mainUI=new MainWindow();
            mainUI->show();
            this->close();
        } else {
            QMessageBox::critical(this, "注销失败", errorMsg);
        }
    }
}

void ui_client::on_pushButtonexit_clicked()
{
    this->close();
}


void ui_client::on_btnExit_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("确认退出登录");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("⚠️ 您确定要退出登录吗？");

    // 自定义按钮
    QPushButton* confirmButton1 = msgBox.addButton("确认退出", QMessageBox::AcceptRole);
    QPushButton* cancelButton1 = msgBox.addButton("取消", QMessageBox::RejectRole);

    // 设置按钮样式
    confirmButton1->setStyleSheet(
        "background-color: #F44336;"
        "color: white;"
        "padding: 8px 20px;"
        "border-radius: 4px;"
        "font-weight: bold;"
        );
    cancelButton1->setStyleSheet(
        "background-color: #E0E0E0;"
        "color: #333;"
        "padding: 8px 20px;"
        "border-radius: 4px;"
        );

    msgBox.exec();
    if (msgBox.clickedButton() == confirmButton1) {    
        QMessageBox::information(this, "感谢使用！","已退出登录");
        mainUI=new MainWindow();
        mainUI->show();
        this->close();
    }
}

