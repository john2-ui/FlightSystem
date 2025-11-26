#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_admin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui_admin* adminUI=new ui_admin();
    adminUI->show();
    this->close();
}

