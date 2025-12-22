#ifndef UI_ADMIN_H
#define UI_ADMIN_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDate>
#include "../backend/backend.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ui_admin;
}
QT_END_NAMESPACE
class MainWindow;
class ui_admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui_admin(QWidget *parent = nullptr);
    ~ui_admin();

private slots:
    // 菜单导航槽函数
    void AddFlightClicked();
    void UpdateFlightClicked();
    void DeleteFlightClicked();
    void AddPlaneClicked();
    void UpdatePlaneClicked();
    void DeletePlaneClicked();
    void AddAirportClicked();
    void AddCityClicked();
    void ReturnLogin();

    // 添加城市
    void on_btn_addcity_clicked();

    // 添加机场
    void on_btn_addairport_clicked();

    // 添加飞机
    void on_btn_addplane_clicked();

    // 添加航班
    void on_btn_addflight_clicked();

    // 更新飞机
    void on_btn_updateplane_clicked();
    void on_updateplaneID_valueChanged(int arg1);

    // 更新航班 - 搜索功能 (已修正为按城市搜索)
    void on_btn_updateflight_clicked();
    void on_btn_searchupdateflight_clicked();
    void on_updateflightTable_cellClicked(int row, int column);
    void on_updateflightID_valueChanged(int arg1);

    // 删除航班 - 搜索功能 (已修正为按城市搜索)
    void on_btn_searchdeleteflight_clicked();
    void on_deleteflightTable_cellClicked(int row, int column);
    void on_deleteflight_clicked();
    void on_deleteflightID_valueChanged(int arg1);

    // 删除飞机
    void on_deleteplaneID_valueChanged(int arg1);
    void on_btn_deleteplane_clicked();

    void on_airplane_currentIndexChanged(int index);

    void on_updateflightplane_currentIndexChanged(int index);

private:
    Ui::ui_admin *ui;
    MainWindow* mainUI=nullptr;

    // 辅助函数
    void loadAirplaneComboBox();
    void loadAirportComboBoxes(); // 仅用于加载具体操作中的机场下拉框
    void loadSearchCityComboBoxes(); // 新增：用于加载搜索栏的城市下拉框
    void setupTableWidget(QTableWidget* table);
    void populateFlightTable(QTableWidget* table, const QList<FlightDetailInfo>& flights);
};

#endif // UI_ADMIN_H
