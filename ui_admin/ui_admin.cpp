#include "ui_admin.h"
#include "ui_ui_admin.h"

ui_admin::ui_admin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui_admin)
{
    ui->setupUi(this);

    connect(ui->AddFlight,&QAction::triggered,this,&ui_admin::AddFlightClicked);
    connect(ui->UpdateFlight,&QAction::triggered,this,&ui_admin::UpdateFlightClicked);
    connect(ui->DeleteFlight,&QAction::triggered,this,&ui_admin::DeleteFlightClicked);
    connect(ui->AddPlane,&QAction::triggered,this,&ui_admin::AddPlaneClicked);
    connect(ui->UpdatePlane,&QAction::triggered,this,&ui_admin::UpdatePlaneClicked);
    connect(ui->DeletePlane,&QAction::triggered,this,&ui_admin::DeletePlaneClicked);

}

ui_admin::~ui_admin()
{
    delete ui;
}

void ui_admin::AddFlightClicked() {
    this->dynamicWidget=ui->page_addflight;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
}

void ui_admin::UpdateFlightClicked() {
    this->dynamicWidget=ui->page_updateflight;
    ui->mainStackedWidget->setCurrentWidget(dynamicWidget);
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
