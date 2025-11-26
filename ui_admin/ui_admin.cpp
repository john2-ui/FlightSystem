#include "ui_admin.h"
#include "ui_ui_admin.h"
#include "../backend/backend.h"
#include <QMessageBox>

ui_admin::ui_admin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui_admin)
{
    ui->setupUi(this);
    ui->mainStackedWidget->setCurrentIndex(0);
    QList<Airplane> airplanelist=Backend::instance().getAllAirplanes();
    ui->airplane->clear();
    if (airplanelist.isEmpty()) {
        ui->airplane->addItem("暂无可用飞机，请先添加飞机",-1);
        ui->airplane->setEnabled(false);
        return;
    }
    foreach (const Airplane& airplane,airplanelist) {
        QString showText=QString("%1,%2").arg(airplane.model()).arg(airplane.id());
        ui->airplane->addItem(showText,airplane.id());
    }
    ui->airplane->setCurrentIndex(0);
    QList<Airport> airportlist=Backend::instance().getAllAirports();
    ui->departAirport->clear();
    ui->arriveAirport->clear();
    if (airportlist.isEmpty()) {
        ui->departAirport->addItem("暂无可用机场，请先添加机场",-1);
        ui->arriveAirport->addItem("暂无可用机场，请先添加机场",-1);
        ui->departAirport->setEnabled(false);
        ui->arriveAirport->setEnabled(false);
        return;
    }
    foreach (const Airport& airport,airportlist) {
        QString showText=QString("%1,%2").arg(airport.name()).arg(airport.code());
        ui->departAirport->addItem(showText,airport.id());
        ui->arriveAirport->addItem(showText,airport.id());
    }
    ui->departAirport->setCurrentIndex(0);
    ui->arriveAirport->setCurrentIndex(0);
    connect(ui->AddFlight,&QAction::triggered,this,&ui_admin::AddFlightClicked);
    connect(ui->UpdateFlight,&QAction::triggered,this,&ui_admin::UpdateFlightClicked);
    connect(ui->DeleteFlight,&QAction::triggered,this,&ui_admin::DeleteFlightClicked);
    connect(ui->AddPlane,&QAction::triggered,this,&ui_admin::AddPlaneClicked);
    connect(ui->UpdatePlane,&QAction::triggered,this,&ui_admin::UpdatePlaneClicked);
    connect(ui->DeletePlane,&QAction::triggered,this,&ui_admin::DeletePlaneClicked);
    connect(ui->AddAirport,&QAction::triggered,this,&ui_admin::AddAirportClicked);
    connect(ui->AddCity,&QAction::triggered,this,&ui_admin::AddCityClicked);
}

ui_admin::~ui_admin()
{
    delete ui;
}

void ui_admin::AddFlightClicked() {
    this->dynamicWidget=ui->page_addflight;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
    QList<Airplane> airplanelist=Backend::instance().getAllAirplanes();
    ui->airplane->clear();
    if (airplanelist.isEmpty()) {
        ui->airplane->addItem("暂无可用飞机，请先添加飞机",-1);
        ui->airplane->setEnabled(false);
        return;
    }
    foreach (const Airplane& airplane,airplanelist) {
        QString showText=QString("%1,%2").arg(airplane.model()).arg(airplane.id());
        ui->airplane->addItem(showText,airplane.id());
    }
    ui->airplane->setCurrentIndex(0);
    QList<Airport> airportlist=Backend::instance().getAllAirports();
    ui->departAirport->clear();
    ui->arriveAirport->clear();
    if (airportlist.isEmpty()) {
        ui->departAirport->addItem("暂无可用机场，请先添加机场",-1);
        ui->arriveAirport->addItem("暂无可用机场，请先添加机场",-1);
        ui->departAirport->setEnabled(false);
        ui->arriveAirport->setEnabled(false);
        return;
    }
    foreach (const Airport& airport,airportlist) {
        QString showText=QString("%1,%2").arg(airport.name()).arg(airport.code());
        ui->departAirport->addItem(showText,airport.id());
        ui->arriveAirport->addItem(showText,airport.id());
    }
    ui->departAirport->setCurrentIndex(0);
    ui->arriveAirport->setCurrentIndex(0);
    ui->departAirport->setEnabled(true);
    ui->arriveAirport->setEnabled(true);
}

void ui_admin::UpdateFlightClicked() {
    this->dynamicWidget=ui->page_updateflight;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
    QList<Airplane> airplanelist=Backend::instance().getAllAirplanes();
    ui->updateflightplane->clear();
    if (airplanelist.isEmpty()) {
        ui->updateflightplane->addItem("暂无可用飞机，请先添加飞机",-1);
        ui->updateflightplane->setEnabled(false);
        return;
    }
    foreach (const Airplane& airplane,airplanelist) {
        QString showText=QString("%1,%2").arg(airplane.model()).arg(airplane.id());
        ui->updateflightplane->addItem(showText,airplane.id());
    }
    ui->updateflightplane->setCurrentIndex(0);
    QList<Airport> airportlist=Backend::instance().getAllAirports();
    ui->updateflightdepartairport->clear();
    ui->updateflightarriveairport->clear();
    if (airportlist.isEmpty()) {
        ui->updateflightdepartairport->addItem("暂无可用机场，请先添加机场",-1);
        ui->updateflightarriveairport->addItem("暂无可用机场，请先添加机场",-1);
        ui->updateflightdepartairport->setEnabled(false);
        ui->updateflightarriveairport->setEnabled(false);
        return;
    }
    foreach (const Airport& airport,airportlist) {
        QString showText=QString("%1,%2").arg(airport.name()).arg(airport.code());
        ui->updateflightdepartairport->addItem(showText,airport.id());
        ui->updateflightarriveairport->addItem(showText,airport.id());
    }
    ui->updateflightdepartairport->setCurrentIndex(0);
    ui->updateflightarriveairport->setCurrentIndex(0);
    ui->updateflightdepartairport->setEnabled(true);
    ui->updateflightarriveairport->setEnabled(true);
}

void ui_admin::DeleteFlightClicked() {
    this->dynamicWidget=ui->page_deleteflight;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
}

void ui_admin::AddPlaneClicked() {
    this->dynamicWidget=ui->page_addplane;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
}

void ui_admin::UpdatePlaneClicked() {
    this->dynamicWidget=ui->page_updateplane;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
}

void ui_admin::DeletePlaneClicked() {
    this->dynamicWidget=ui->page_deleteplane;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
}

void ui_admin::AddAirportClicked() {
    this->dynamicWidget=ui->page_addairport;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
    QList<City> cityList=Backend::instance().getAllCities();
    ui->newairportcity->clear();
    if (cityList.isEmpty()) {
        ui->newairportcity->addItem("暂无可用城市，请先添加城市",-1);
        ui->newairportcity->setEnabled(false);
        return;
    }
    foreach (const City& city,cityList) {
        QString showText=QString("%1,%2").arg(city.name()).arg(city.code());
        ui->newairportcity->addItem(showText,city.id());
    }
    ui->newairportcity->setCurrentIndex(0);
    ui->newairportcity->setEnabled(true);
}

void ui_admin::AddCityClicked() {
    this->dynamicWidget=ui->page_addcity;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
}

void ui_admin::on_btn_addcity_clicked()
{
    Backend& backend=Backend::instance();
    City newcity=City();
    QString cityname=ui->newcityname->text().trimmed();
    QString citycode=ui->newcitycode->text().trimmed();
    QString citycountry=ui->newcitycountry->text().trimmed();
    if (cityname.isEmpty()||citycode.isEmpty()||citycountry.isEmpty()) {
        QMessageBox::warning(this,"警告","城市名称，代码和所属国家不能为空");
    }
    int cityId=backend.addCity(newcity);
    if (cityId>0) {
        qDebug()<<"添加城市成功，Id:"<<cityId;
    } else {
        qDebug()<<"添加城市失败！";
    }
}


void ui_admin::on_btn_addairport_clicked()
{
    QString airportname=ui->newairportname->text().trimmed();
    QString airportcode=ui->newairportcode->text().trimmed();
    int terminalCount=ui->newairportterminals->value();
    int selectedCityId=ui->newairportcity->currentData().toInt();

    if (airportname.isEmpty()||airportcode.isEmpty()) {
        QMessageBox::warning(this,"警告","机场名称和代码不能为空！");
        return;
    }
    if (selectedCityId == -1) {
        QMessageBox::warning(this,"警告","请先添加城市再创建机场");
        return;
    }
    Airport newairport;
    newairport.setName(airportname);
    newairport.setCode(airportcode);
    newairport.setTerminalCount(terminalCount);
    newairport.setCityId(selectedCityId);
    int airportId=Backend::instance().addAirport(newairport);
    if (airportId>0) {
        qDebug()<<"添加机场成功，Id:"<<airportId;
    } else {
        qDebug()<<"添加机场失败！";
    }
}


void ui_admin::on_btn_addplane_clicked()
{
    QString planemodel=ui->newplanemodel->text().trimmed();
    int seatsEconomy=ui->newplaneseatsEconomy->value();
    int seatsBusiness=ui->newplaneseatsBusiness->value();
    int seatsFirst=ui->newplaneseatsFirst->value();
    Airplane newplane;
    newplane.setModel(planemodel);
    newplane.setSeatsEconomy(seatsEconomy);
    newplane.setSeatsBusiness(seatsBusiness);
    newplane.setSeatsFirst(seatsFirst);
    int AirplaneId=Backend::instance().addAirplane(newplane);
    if (AirplaneId>0) {
        qDebug()<<"添加飞机成功，Id:"<<AirplaneId;
    } else {
        qDebug()<<"添加飞机失败！";
    }
}


void ui_admin::on_btn_addflight_clicked()
{
    QString flightNo=ui->flightNo->text().trimmed();
    int airplaneId=ui->airplane->currentIndex()+1;
    int departairportId=ui->departAirport->currentIndex()+1;
    int arriveairportId=ui->arriveAirport->currentIndex()+1;
    QDateTime departtime=ui->departTime->dateTime();
    QDateTime arrivetime=ui->arriveTime->dateTime();
    QString status;
    if (ui->rbtn_normal->isChecked()) {
        status="normal";
    } else if (ui->rbtn_delayed->isChecked()) {
        status="delayed";
    } else if (ui->rbtn_cancelled->isChecked()) {
        status="cancelled";
    } else if (ui->rbtn_scheduled->isChecked()) {
        status="scheduled";
    }
    Flight newflight;
    newflight.setFlightNo(flightNo);
    newflight.setAirplaneId(airplaneId);
    newflight.setDepartAirportId(departairportId);
    newflight.setArriveAirportId(arriveairportId);
    newflight.setDepartTime(departtime);
    newflight.setArriveTime(arrivetime);
    newflight.setStatus(status);
    QString errorMsg;
    int flightId=Backend::instance().addFlight(newflight,errorMsg);
    if (flightId>0) {
        qDebug()<<"添加航班成功，ID:"<<flightId;
    } else {
        qDebug()<<"添加失败:"<<errorMsg;
    }
}


void ui_admin::on_btn_updateplane_clicked()
{
    int ID=ui->updateplaneID->value();
    QString model=ui->updateplanemodel->text().trimmed();
    int seatseconomy=ui->updateplaneseatsEconomy->value();
    int seatsbusiness=ui->updateplaneseatsBusiness->value();
    int seatsfirst=ui->updateplaneseatsFirst->value();
    Airplane updateplane;
    updateplane.setId(ID);
    updateplane.setModel(model);
    updateplane.setSeatsEconomy(seatseconomy);
    updateplane.setSeatsBusiness(seatsbusiness);
    updateplane.setSeatsFirst(seatsfirst);
    if (Backend::instance().updateAirplane(updateplane)) {
        qDebug()<<"更新飞机成功";
    } else {
        qDebug()<<"更新飞机失败";
    }
}


void ui_admin::on_updateplaneID_valueChanged(int arg1)
{
    Airplane updateplane=Backend::instance().getAirplaneById(arg1);
    if (updateplane.model().isEmpty()) {
        ui->updateplanemodel->setText("该飞机不存在");
        return;
    }
    ui->updateplanemodel->setText(updateplane.model());
    ui->updateplaneseatsEconomy->setValue(updateplane.seatsEconomy());
    ui->updateplaneseatsBusiness->setValue(updateplane.seatsBusiness());
    ui->updateplaneseatsFirst->setValue(updateplane.seatsFirst());
}



void ui_admin::on_btn_updateflight_clicked()
{
    int ID=ui->updateflightID->value();
    QString updateflightNo=ui->updateflightNo->text().trimmed();
    int updateflightplaneId=ui->updateflightplane->currentIndex()+1;
    int updateflightdepartairportId=ui->updateflightdepartairport->currentIndex()+1;
    int updateflightarriveairportId=ui->updateflightarriveairport->currentIndex()+1;
    QDateTime updateflightdeparttime=ui->updateflightdeparttime->dateTime();
    QDateTime updateflightarrivetime=ui->updateflightarrivetime->dateTime();
    QString updateflightstatus;
    if (ui->rbtn_normal_update->isChecked()) {
        updateflightstatus="normal";
    } else if (ui->rbtn_delayed_update->isChecked()) {
        updateflightstatus="delayed";
    } else if (ui->rbtn_cancelled_update->isChecked()) {
        updateflightstatus="cancelled";
    } else if (ui->rbtn_scheduled_update->isChecked()) {
        updateflightstatus="scheduled";
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
    if (Backend::instance().updateFlight(updateflight,errormsg)){
        qDebug()<<"更新航班成功";
    } else {
        qDebug()<<errormsg;
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


void ui_admin::on_deleteflight_clicked()
{
    QString errormsg;
    if (Backend::instance().deleteFlight(ui->deleteflightID->value(),errormsg)) {
        qDebug()<<"删除航班成功";
    } else {
        qDebug()<<errormsg;
    }
}


void ui_admin::on_deleteplaneID_valueChanged(int arg1)
{
    Airplane deleteplane=Backend::instance().getAirplaneById(arg1);
    if (deleteplane.model().isEmpty()) {
        ui->deleteplanemodel->setText("该飞机不存在");
        return;
    }
    ui->deleteplanemodel->setText(deleteplane.model());
    ui->deleteplaneseatseconomy->setText(QString("%1").arg(deleteplane.seatsEconomy()));
    ui->deleteplaneseatsbusiness->setText(QString("%1").arg(deleteplane.seatsBusiness()));
    ui->deleteplaneseatsfirst->setText(QString("%1").arg(deleteplane.seatsFirst()));
}


void ui_admin::on_btn_deleteplane_clicked()
{
    if (Backend::instance().deleteAirplane(ui->deleteplaneID->value())) {
        qDebug()<<"删除飞机成功";
    } else {
        qDebug()<<"删除飞机失败";
    }
}

