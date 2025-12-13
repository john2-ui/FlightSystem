#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class ui_admin;
class ui_client;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 登录页面
    void on_btnLogin_clicked();
    void on_btnToRegister_clicked();

    // 注册页面
    void on_btnRegister_clicked();
    void on_btnBackToLogin_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    ui_admin* adminUI = nullptr;
    ui_client* clientUI = nullptr;

    // 私有方法
    bool checkLogin(const QString &username, const QString &password);
    bool registerUser(const QString &username, const QString &password);
    bool isUsernameExists(const QString &username);
    void showLoginMessage(const QString &message, bool isSuccess = false);
    void showRegisterMessage(const QString &message, bool isSuccess = false);
};

#endif // MAINWINDOW_H
