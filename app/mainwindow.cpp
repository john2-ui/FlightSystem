#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_admin.h"
#include "ui_client.h"
#include "../backend/backend.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口固定大小
    setFixedSize(800, 600);

    // 默认显示登录页面
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (adminUI) delete adminUI;
    if (clientUI) delete clientUI;
}

// ==================== 登录功能 ====================
void MainWindow::on_btnLogin_clicked()
{
    QString username = ui->editUsername->text().trimmed();
    QString password = ui->editPassword->text().trimmed();

    // 检查输入
    if (username.isEmpty()) {
        ui->labelLoginStatus->setText("请输入用户名");
        ui->labelLoginStatus->setStyleSheet("color: red;");
        return;
    }

    if (password.isEmpty()) {
        ui->labelLoginStatus->setText("请输入密码");
        ui->labelLoginStatus->setStyleSheet("color: red;");
        return;
    }
    int userId;
    bool isAdmin;
    QString errorMsg;
    if (Backend::instance().loginUser(username, password, userId, isAdmin, errorMsg)) {
        ui->labelLoginStatus->setText("登录成功！");
        ui->labelLoginStatus->setStyleSheet("color: green;");
        QTimer::singleShot(500, [this, userId, username, isAdmin]() {
            if (isAdmin) {
                // 管理员
                adminUI = new ui_admin();
                adminUI->show();
            } else {
                // 普通用户
                clientUI = new ui_client();
                // 传递用户信息
                clientUI->setCurrentUserId(userId);
                clientUI->setCurrentUsername(username);
                clientUI->show();
            }
            this->close();
        });

    } else {
        ui->labelLoginStatus->setText(errorMsg);
        ui->labelLoginStatus->setStyleSheet("color: red;");
    }
}

// 切换到注册页面
void MainWindow::on_btnToRegister_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->editRegUsername->setFocus();
}

//注册功能
void MainWindow::on_btnRegister_clicked()
{
    QString username = ui->editRegUsername->text().trimmed();
    QString password = ui->editRegPassword->text().trimmed();
    QString confirm = ui->editConfirmPassword->text().trimmed();

    // 检查输入
    if (username.isEmpty()) {
        ui->labelRegStatus->setText("请输入用户名");
        ui->labelRegStatus->setStyleSheet("color: red;");
        return;
    }

    if (password.isEmpty()) {
        ui->labelRegStatus->setText("请输入密码");
        ui->labelRegStatus->setStyleSheet("color: red;");
        return;
    }

    if (confirm.isEmpty()) {
        ui->labelRegStatus->setText("请确认密码");
        ui->labelRegStatus->setStyleSheet("color: red;");
        return;
    }

    if (password != confirm) {
        ui->labelRegStatus->setText("两次密码输入不一致");
        ui->labelRegStatus->setStyleSheet("color: red;");
        return;
    }

    // 调用Backend的注册方法（只能是普通用户）
    QString errorMsg;
    bool success = Backend::instance().registerUser(username, password, false, errorMsg);

    if (success) {
        ui->labelRegStatus->setText("注册成功！请登录");
        ui->labelRegStatus->setStyleSheet("color: green;");

        // 延迟1.5秒后返回登录页面
        QTimer::singleShot(500, [this, username]() {
            ui->stackedWidget->setCurrentIndex(0);
            ui->editUsername->setText(username);
            ui->editPassword->clear();
            ui->editPassword->setFocus();
        });

    } else {
        ui->labelRegStatus->setText(errorMsg);
        ui->labelRegStatus->setStyleSheet("color: red;");
    }
}

// 返回登录页面
void MainWindow::on_btnBackToLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->editUsername->setFocus();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

