#include "ui_admin.h"
#include "ui_ui_admin.h"
#include "mainwindow.h"
#include "../backend/backend.h"
#include <QMessageBox>
#include <QHeaderView>

ui_admin::ui_admin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui_admin)
{
    ui->setupUi(this);
    ui->mainStackedWidget->setCurrentIndex(0);

    // 设置表格样式
    setupTableWidget(ui->updateflightTable);
    setupTableWidget(ui->deleteflightTable);

    // 初始化下拉框
    loadAirplaneComboBox();
    loadAirportComboBoxes();
    loadSearchCityComboBoxes(); // 加载城市搜索框

    // 连接菜单信号
    connect(ui->AddFlight, &QAction::triggered, this, &ui_admin::AddFlightClicked);
    connect(ui->UpdateFlight, &QAction::triggered, this, &ui_admin::UpdateFlightClicked);
    connect(ui->DeleteFlight, &QAction::triggered, this, &ui_admin::DeleteFlightClicked);
    connect(ui->AddPlane, &QAction::triggered, this, &ui_admin::AddPlaneClicked);
    connect(ui->UpdatePlane, &QAction::triggered, this, &ui_admin::UpdatePlaneClicked);
    connect(ui->DeletePlane, &QAction::triggered, this, &ui_admin::DeletePlaneClicked);
    connect(ui->AddAirport, &QAction::triggered, this, &ui_admin::AddAirportClicked);
    connect(ui->AddCity, &QAction::triggered, this, &ui_admin::AddCityClicked);
    connect(ui->act_returnlogin, &QAction::triggered, this, &ui_admin::ReturnLogin);
}

ui_admin::~ui_admin()
{
    delete ui;
}

void ui_admin::setupTableWidget(QTableWidget* table)
{
    table->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "航班号" << "飞机型号" << "起飞机场" << "到达机场"
            << "起飞时间" << "到达时间" << "状态";
    table->setHorizontalHeaderLabels(headers);

    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);

    // 设置列宽
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 100);
    table->setColumnWidth(2, 120);
    table->setColumnWidth(3, 150);
    table->setColumnWidth(4, 150);
}

void ui_admin::loadAirplaneComboBox()
{
    QList<Airplane> airplanelist = Backend::instance().getAllAirplanes();
    ui->airplane->clear();
    ui->updateflightplane->clear(); // 确保更新框也被清空

    if (airplanelist.isEmpty()) {
        ui->airplane->addItem("暂无可用飞机，请先添加飞机", -1);
        ui->airplane->setEnabled(false);
        return;
    }

    foreach (const Airplane& airplane, airplanelist) {
        QString showText = QString("%1 (ID:%2)").arg(airplane.model()).arg(airplane.id());
        ui->airplane->addItem(showText, airplane.id());
        ui->updateflightplane->addItem(showText, airplane.id());
    }
    ui->airplane->setEnabled(true);
    ui->updateflightplane->setEnabled(true);
}

// 修改：此函数仅加载具体航班操作（添加/修改）所需的具体机场
void ui_admin::loadAirportComboBoxes()
{
    QList<Airport> airportlist = Backend::instance().getAllAirports();

    // 清空具体操作的机场下拉框
    ui->departAirport->clear();
    ui->arriveAirport->clear();
    ui->updateflightdepartairport->clear();
    ui->updateflightarriveairport->clear();

    if (airportlist.isEmpty()) {
        QString msg = "暂无可用机场，请先添加机场";
        ui->departAirport->addItem(msg, -1);
        ui->arriveAirport->addItem(msg, -1);
        ui->updateflightdepartairport->addItem(msg, -1);
        ui->updateflightarriveairport->addItem(msg, -1);
        return;
    }

    foreach (const Airport& airport, airportlist) {
        QString showText = QString("%1 (%2)").arg(airport.name()).arg(airport.code());
        // 这里的 userData 存 ID，因为添加/修改接口需要 airportId
        ui->departAirport->addItem(showText, airport.id());
        ui->arriveAirport->addItem(showText, airport.id());
        ui->updateflightdepartairport->addItem(showText, airport.id());
        ui->updateflightarriveairport->addItem(showText, airport.id());
    }
}

// 新增：加载搜索所需的城市下拉框
void ui_admin::loadSearchCityComboBoxes()
{
    QList<City> cityList = Backend::instance().getAllCities();

    ui->searchUpdateDepartCity->clear();
    ui->searchUpdateArriveCity->clear();
    ui->searchDeleteDepartCity->clear();
    ui->searchDeleteArriveCity->clear();

    if (cityList.isEmpty()) {
        QString msg = "暂无城市";
        ui->searchUpdateDepartCity->addItem(msg, "");
        ui->searchUpdateArriveCity->addItem(msg, "");
        ui->searchDeleteDepartCity->addItem(msg, "");
        ui->searchDeleteArriveCity->addItem(msg, "");
        return;
    }

    // 添加"全部"选项 (代码为空字符串或特定标识，视后端处理而定，这里假设传空字符串代表不筛选)
    // 注意：如果后端searchFlights不支持空字符串查全部，这里需要后端配合。
    // 但根据需求，我们先列出所有具体城市。

    foreach (const City& city, cityList) {
        QString showText = QString("%1 (%2)").arg(city.name()).arg(city.code());
        // 这里的 userData 存 Code，因为搜索接口 searchFlights 需要 cityCode
        ui->searchUpdateDepartCity->addItem(showText, city.code());
        ui->searchUpdateArriveCity->addItem(showText, city.code());
        ui->searchDeleteDepartCity->addItem(showText, city.code());
        ui->searchDeleteArriveCity->addItem(showText, city.code());
    }
}


void ui_admin::populateFlightTable(QTableWidget* table, const QList<FlightDetailInfo>& flights)
{
    table->setRowCount(0);

    foreach (const FlightDetailInfo& flight, flights) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(flight.flightId)));
        table->setItem(row, 1, new QTableWidgetItem(flight.flightNo));
        table->setItem(row, 2, new QTableWidgetItem(flight.airplaneModel));
        table->setItem(row, 3, new QTableWidgetItem(flight.departAirportName));
        table->setItem(row, 4, new QTableWidgetItem(flight.arriveAirportName));
        table->setItem(row, 5, new QTableWidgetItem(flight.departTime.toString("yyyy-MM-dd hh:mm")));
        table->setItem(row, 6, new QTableWidgetItem(flight.arriveTime.toString("yyyy-MM-dd hh:mm")));
        table->setItem(row, 7, new QTableWidgetItem(flight.status));
    }
}

void ui_admin::AddFlightClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_addflight);
    loadAirplaneComboBox();
    loadAirportComboBoxes();
}

void ui_admin::UpdateFlightClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_updateflight);
    loadAirplaneComboBox();
    loadAirportComboBoxes();
    loadSearchCityComboBoxes(); // 刷新搜索用的城市列表
    ui->updateflightTable->setRowCount(0);
}

void ui_admin::DeleteFlightClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_deleteflight);
    loadSearchCityComboBoxes(); // 刷新搜索用的城市列表
    ui->deleteflightTable->setRowCount(0);
}

void ui_admin::AddPlaneClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_addplane);
}

void ui_admin::UpdatePlaneClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_updateplane);
}

void ui_admin::DeletePlaneClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_deleteplane);
}

void ui_admin::AddAirportClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_addairport);
    QList<City> cityList = Backend::instance().getAllCities();
    ui->newairportcity->clear();

    if (cityList.isEmpty()) {
        ui->newairportcity->addItem("暂无可用城市，请先添加城市", -1);
        ui->newairportcity->setEnabled(false);
        return;
    }

    foreach (const City& city, cityList) {
        QString showText = QString("%1 (%2)").arg(city.name()).arg(city.code());
        ui->newairportcity->addItem(showText, city.id());
    }
    ui->newairportcity->setEnabled(true);
}

void ui_admin::AddCityClicked() {
    ui->mainStackedWidget->setCurrentWidget(ui->page_addcity);
}

void ui_admin::on_btn_addcity_clicked()
{
    QString cityname = ui->newcityname->text().trimmed();
    QString citycode = ui->newcitycode->text().trimmed();
    QString citycountry = ui->newcitycountry->text().trimmed();

    if (cityname.isEmpty() || citycode.isEmpty() || citycountry.isEmpty()) {
        QMessageBox::warning(this, "警告", "城市名称，代码和所属国家不能为空");
        return;
    }

    int cityId = Backend::instance().addCity(cityname, citycode, citycountry);
    if (cityId > 0) {
        QMessageBox::information(this, "成功", "添加城市成功！ID: " + QString::number(cityId));
        ui->newcityname->clear();
        ui->newcitycode->clear();
        ui->newcitycountry->clear();
    } else {
        QMessageBox::critical(this, "失败", "添加城市失败！");
    }
}

void ui_admin::on_btn_addairport_clicked()
{
    QString airportname = ui->newairportname->text().trimmed();
    QString airportcode = ui->newairportcode->text().trimmed();
    int terminalCount = ui->newairportterminals->value();
    int selectedCityId = ui->newairportcity->currentData().toInt();

    if (airportname.isEmpty() || airportcode.isEmpty()) {
        QMessageBox::warning(this, "警告", "机场名称和代码不能为空！");
        return;
    }

    if (selectedCityId == -1) {
        QMessageBox::warning(this, "警告", "请先添加城市再创建机场");
        return;
    }

    int airportId = Backend::instance().addAirport(airportname, airportcode, selectedCityId, terminalCount);
    if (airportId > 0) {
        QMessageBox::information(this, "成功", "添加机场成功！ID: " + QString::number(airportId));
        ui->newairportname->clear();
        ui->newairportcode->clear();
        ui->newairportterminals->setValue(1);
    } else {
        QMessageBox::critical(this, "失败", "添加机场失败！");
    }
}

void ui_admin::on_btn_addplane_clicked()
{
    QString planemodel = ui->newplanemodel->text().trimmed();
    int seatsEconomy = ui->newplaneseatsEconomy->value();
    int seatsBusiness = ui->newplaneseatsBusiness->value();
    int seatsFirst = ui->newplaneseatsFirst->value();

    if (planemodel.isEmpty()) {
        QMessageBox::warning(this, "警告", "飞机型号不能为空！");
        return;
    }

    int AirplaneId = Backend::instance().addAirplane(planemodel, seatsEconomy, seatsBusiness, seatsFirst);
    if (AirplaneId > 0) {
        QMessageBox::information(this, "成功", "添加飞机成功！ID: " + QString::number(AirplaneId));
        ui->newplanemodel->clear();
        ui->newplaneseatsEconomy->setValue(0);
        ui->newplaneseatsBusiness->setValue(0);
        ui->newplaneseatsFirst->setValue(0);
    } else {
        QMessageBox::critical(this, "失败", "添加飞机失败！");
    }
}

void ui_admin::on_btn_addflight_clicked()
{
    QString flightNo = ui->flightNo->text().trimmed();
    int airplaneId = ui->airplane->currentData().toInt();
    int departairportId = ui->departAirport->currentData().toInt();
    int arriveairportId = ui->arriveAirport->currentData().toInt();
    QDateTime departtime = ui->departTime->dateTime();
    QDateTime arrivetime = ui->arriveTime->dateTime();

    if (flightNo.isEmpty()) {
        QMessageBox::warning(this, "警告", "航班编号不能为空！");
        return;
    }

    QString status;
    if (ui->rbtn_normal->isChecked()) {
        status = "normal";
    } else if (ui->rbtn_delayed->isChecked()) {
        status = "delayed";
    } else if (ui->rbtn_cancelled->isChecked()) {
        status = "cancelled";
    } else if (ui->rbtn_scheduled->isChecked()) {
        status = "scheduled";
    }

    QString errorMsg;
    int flightId = Backend::instance().addFlight(
        flightNo,
        airplaneId,
        departairportId,
        arriveairportId,
        departtime,
        arrivetime,
        status,
        errorMsg
    );
    if (flightId > 0) {
        QMessageBox::information(this, "成功", "添加航班成功！ID: " + QString::number(flightId));
        ui->flightNo->clear();
    } else {
        QMessageBox::critical(this, "失败", "添加失败: " + errorMsg);
    }
}

// 修正：使用城市代码进行搜索
void ui_admin::on_btn_searchupdateflight_clicked()
{
    // 获取城市代码而不是机场代码
    QString fromCityCode = ui->searchUpdateDepartCity->currentData().toString();
    QString toCityCode = ui->searchUpdateArriveCity->currentData().toString();
    QDate date = ui->searchUpdateDate->date();

    // 调试提示：如果需要，可以取消注释查看实际传了什么
    // qDebug() << "Searching: " << fromCityCode << " -> " << toCityCode << " at " << date;

    QList<FlightDetailInfo> flights = Backend::instance().searchFlights(fromCityCode, toCityCode, date);
    populateFlightTable(ui->updateflightTable, flights);

    if (flights.isEmpty()) {
        QMessageBox::information(this, "提示", "没有找到符合条件的航班");
    }
}

void ui_admin::on_updateflightTable_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    int flightId = ui->updateflightTable->item(row, 0)->text().toInt();
    FlightDetailInfo flight = Backend::instance().getFlightDetail(flightId);

    if (flight.flightNo.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取航班详情");
        return;
    }

    ui->updateflightID->setValue(flightId);
    ui->updateflightNo->setText(flight.flightNo);

    // 设置编辑区的机场下拉框（注意：这里依然需要匹配机场名称，因为编辑是针对具体机场的）
    int departIdx = ui->updateflightdepartairport->findText(flight.departAirportName, Qt::MatchContains);
    int arriveIdx = ui->updateflightarriveairport->findText(flight.arriveAirportName, Qt::MatchContains);
    if (departIdx >= 0) ui->updateflightdepartairport->setCurrentIndex(departIdx);
    if (arriveIdx >= 0) ui->updateflightarriveairport->setCurrentIndex(arriveIdx);

    // 设置飞机
    int planeIdx = ui->updateflightplane->findText(flight.airplaneModel, Qt::MatchContains);
    if (planeIdx >= 0) ui->updateflightplane->setCurrentIndex(planeIdx);

    ui->updateflightdeparttime->setDateTime(flight.departTime);
    ui->updateflightarrivetime->setDateTime(flight.arriveTime);

    // 设置状态
    if (flight.status == "normal") {
        ui->rbtn_normal_update->setChecked(true);
    } else if (flight.status == "delayed") {
        ui->rbtn_delayed_update->setChecked(true);
    } else if (flight.status == "cancelled") {
        ui->rbtn_cancelled_update->setChecked(true);
    } else {
        ui->rbtn_scheduled_update->setChecked(true);
    }
    //设置余票
    // int tickets=0;
    // foreach (auto& ticket,flight.tickets) {
    //     tickets+=ticket.remainSeats;
    // }
    // ui->updateflighttickets->setValue(tickets);
}

void ui_admin::on_btn_updateflight_clicked()
{
    int ID = ui->updateflightID->value();
    QString updateflightNo = ui->updateflightNo->text().trimmed();

    if (updateflightNo.isEmpty()) {
        QMessageBox::warning(this, "警告", "航班编号不能为空！");
        return;
    }

    int updateflightplaneId = ui->updateflightplane->currentData().toInt();
    int updateflightdepartairportId = ui->updateflightdepartairport->currentData().toInt();
    int updateflightarriveairportId = ui->updateflightarriveairport->currentData().toInt();
    QDateTime updateflightdeparttime = ui->updateflightdeparttime->dateTime();
    QDateTime updateflightarrivetime = ui->updateflightarrivetime->dateTime();

    QString updateflightstatus;
    if (ui->rbtn_normal_update->isChecked()) {
        updateflightstatus = "normal";
    } else if (ui->rbtn_delayed_update->isChecked()) {
        updateflightstatus = "delayed";
    } else if (ui->rbtn_cancelled_update->isChecked()) {
        updateflightstatus = "cancelled";
    } else if (ui->rbtn_scheduled_update->isChecked()) {
        updateflightstatus = "scheduled";
    }

    Flight updateflight;
    updateflight.setId(ID);
    updateflight.setFlightNo(updateflightNo);
    updateflight.setAirplaneId(updateflightplaneId);
    updateflight.setDepartAirportId(updateflightdepartairportId);
    updateflight.setArriveAirportId(updateflightarriveairportId);
    updateflight.setDepartTime(updateflightdeparttime);
    updateflight.setArriveTime(updateflightarrivetime);
    updateflight.setStatus(updateflightstatus);

    QString errormsg;
    if (Backend::instance().updateFlight(updateflight, errormsg)) {
        QMessageBox::information(this, "成功", "更新航班成功！");
        on_btn_searchupdateflight_clicked(); // 刷新表格
    } else {
        QMessageBox::critical(this, "失败", errormsg);
    }
}

// 修正：使用城市代码进行搜索
void ui_admin::on_btn_searchdeleteflight_clicked()
{
    QString fromCityCode = ui->searchDeleteDepartCity->currentData().toString();
    QString toCityCode = ui->searchDeleteArriveCity->currentData().toString();
    QDate date = ui->searchDeleteDate->date();

    QList<FlightDetailInfo> flights = Backend::instance().searchFlights(fromCityCode, toCityCode, date);
    populateFlightTable(ui->deleteflightTable, flights);

    if (flights.isEmpty()) {
        QMessageBox::information(this, "提示", "没有找到符合条件的航班");
    }
}

void ui_admin::on_deleteflightTable_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    int flightId = ui->deleteflightTable->item(row, 0)->text().toInt();
    FlightDetailInfo flight = Backend::instance().getFlightDetail(flightId);

    if (flight.flightNo.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取航班详情");
        return;
    }

    ui->deleteflightID->setValue(flightId);
    ui->deleteflightNo->setText(flight.flightNo);
    ui->deleteflightplane->setText(flight.airplaneModel);
    ui->deleteflightdepartairport->setText(flight.departAirportName);
    ui->deleteflightarriveairport->setText(flight.arriveAirportName);
    ui->deleteflightdeparttime->setText(flight.departTime.toString("yyyy-MM-dd hh:mm"));
    ui->deleteflightarrivetime->setText(flight.arriveTime.toString("yyyy-MM-dd hh:mm"));
    ui->deleteflightstatus->setText(flight.status);
}

void ui_admin::on_deleteflight_clicked()
{
    int flightId = ui->deleteflightID->value();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除航班 %1 吗？").arg(ui->deleteflightNo->text()),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        QString errormsg;
        if (Backend::instance().deleteFlight(flightId, errormsg)) {
            QMessageBox::information(this, "成功", "删除航班成功！");
            on_btn_searchdeleteflight_clicked(); // 刷新表格
            // 清空详情
            ui->deleteflightNo->clear();
            ui->deleteflightplane->clear();
            ui->deleteflightdepartairport->clear();
            ui->deleteflightarriveairport->clear();
            ui->deleteflightdeparttime->clear();
            ui->deleteflightarrivetime->clear();
            ui->deleteflightstatus->clear();
        } else {
            QMessageBox::critical(this, "失败", errormsg);
        }
    }
}

void ui_admin::on_updateplaneID_valueChanged(int arg1)
{
    Airplane updateplane = Backend::instance().getAirplaneById(arg1);
    if (updateplane.model().isEmpty()) {
        ui->updateplanemodel->setText("该飞机不存在");
        return;
    }
    ui->updateplanemodel->setText(updateplane.model());
    ui->updateplaneseatsEconomy->setValue(updateplane.seatsEconomy());
    ui->updateplaneseatsBusiness->setValue(updateplane.seatsBusiness());
    ui->updateplaneseatsFirst->setValue(updateplane.seatsFirst());
}

void ui_admin::on_btn_updateplane_clicked()
{
    int ID = ui->updateplaneID->value();
    QString model = ui->updateplanemodel->text().trimmed();

    if (model.isEmpty() || model == "该飞机不存在") {
        QMessageBox::warning(this, "警告", "请输入有效的飞机ID");
        return;
    }

    int seatseconomy = ui->updateplaneseatsEconomy->value();
    int seatsbusiness = ui->updateplaneseatsBusiness->value();
    int seatsfirst = ui->updateplaneseatsFirst->value();

    Airplane updateplane;
    updateplane.setId(ID);
    updateplane.setModel(model);
    updateplane.setSeatsEconomy(seatseconomy);
    updateplane.setSeatsBusiness(seatsbusiness);
    updateplane.setSeatsFirst(seatsfirst);

    if (Backend::instance().updateAirplane(updateplane)) {
        QMessageBox::information(this, "成功", "更新飞机成功！");
    } else {
        QMessageBox::critical(this, "失败", "更新飞机失败！");
    }
}

void ui_admin::on_deleteplaneID_valueChanged(int arg1)
{
    Airplane deleteplane = Backend::instance().getAirplaneById(arg1);
    if (deleteplane.model().isEmpty()) {
        ui->deleteplanemodel->setText("该飞机不存在");
        return;
    }
    ui->deleteplanemodel->setText(deleteplane.model());
    ui->deleteplaneseatseconomy->setText(QString::number(deleteplane.seatsEconomy()));
    ui->deleteplaneseatsbusiness->setText(QString::number(deleteplane.seatsBusiness()));
    ui->deleteplaneseatsfirst->setText(QString::number(deleteplane.seatsFirst()));
}

void ui_admin::on_btn_deleteplane_clicked()
{
    QString model = ui->deleteplanemodel->text();

    if (model.isEmpty() || model == "该飞机不存在") {
        QMessageBox::warning(this, "警告", "请输入有效的飞机ID");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除飞机 %1 吗？").arg(model),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (Backend::instance().deleteAirplane(ui->deleteplaneID->value())) {
            QMessageBox::information(this, "成功", "删除飞机成功！");
            ui->deleteplanemodel->clear();
            ui->deleteplaneseatseconomy->clear();
            ui->deleteplaneseatsbusiness->clear();
            ui->deleteplaneseatsfirst->clear();
        } else {
            QMessageBox::critical(this, "失败", "删除飞机失败！");
        }
    }
}

void ui_admin::on_updateflightID_valueChanged(int arg1)
{
    FlightDetailInfo updateflight=Backend::instance().getFlightDetail(arg1);
    if (updateflight.flightNo.isEmpty()) {
        ui->updateflightNo->setText("该航班不存在");
        return;
    }
    ui->updateflightNo->setText(updateflight.flightNo);
    ui->updateflightdepartairport->setCurrentIndex(ui->updateflightdepartairport->findText(updateflight.departAirportName,Qt::MatchContains));
    ui->updateflightarriveairport->setCurrentIndex(ui->updateflightarriveairport->findText(updateflight.arriveAirportName,Qt::MatchContains));
    ui->updateflightdeparttime->setDateTime(updateflight.departTime);
    ui->updateflightarrivetime->setDateTime(updateflight.arriveTime);
    if (updateflight.status=="normal") {
        ui->rbtn_normal_update->setChecked(true);
    } else if (updateflight.status=="delayed") {
        ui->rbtn_delayed_update->setChecked(true);
    } else if (updateflight.status=="cancelled") {
        ui->rbtn_cancelled_update->setChecked(true);
    } else {
        ui->rbtn_scheduled_update->setChecked(true);
    }
    ui->updateflightplane->setCurrentIndex(ui->updateflightplane->findText(updateflight.airplaneModel,Qt::MatchContains));
}

void ui_admin::on_deleteflightID_valueChanged(int arg1)
{
    FlightDetailInfo deleteflight=Backend::instance().getFlightDetail(arg1);
    if (deleteflight.flightNo.isEmpty()) {
        ui->deleteflightNo->setText("该航班不存在");
        return;
    }
    ui->deleteflightNo->setText(deleteflight.flightNo);
    ui->deleteflightplane->setText(deleteflight.airplaneModel);
    ui->deleteflightdepartairport->setText(deleteflight.departAirportName);
    ui->deleteflightarriveairport->setText(deleteflight.arriveAirportName);
    ui->deleteflightdeparttime->setText(deleteflight.departTime.toString());
    ui->deleteflightarrivetime->setText(deleteflight.arriveTime.toString());
    ui->deleteflightstatus->setText(deleteflight.status);
}

void ui_admin::ReturnLogin(){
    mainUI=new MainWindow();
    mainUI->show();
    this->close();
}
