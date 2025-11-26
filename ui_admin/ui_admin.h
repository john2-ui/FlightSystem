#ifndef UI_ADMIN_H
#define UI_ADMIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class ui_admin;
}
QT_END_NAMESPACE

class ui_admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui_admin(QWidget *parent = nullptr);
    ~ui_admin();

private slots:
    void AddFlightClicked();
    void UpdateFlightClicked();
    void DeleteFlightClicked();
    void AddPlaneClicked();
    void UpdatePlaneClicked();
    void DeletePlaneClicked();
    void AddAirportClicked();
    void AddCityClicked();


    void on_btn_addcity_clicked();

    void on_btn_addairport_clicked();

    void on_btn_addplane_clicked();

    void on_btn_addflight_clicked();

    void on_btn_updateplane_clicked();

    void on_updateplaneID_valueChanged(int arg1);


    void on_btn_updateflight_clicked();

    void on_updateflightID_valueChanged(int arg1);

    void on_deleteflightID_valueChanged(int arg1);

    void on_deleteflight_clicked();

    void on_deleteplaneID_valueChanged(int arg1);

    void on_btn_deleteplane_clicked();

private:
    Ui::ui_admin *ui;
    QWidget* dynamicWidget;
};
#endif // UI_ADMIN_H
