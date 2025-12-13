#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QMainWindow>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>
#include <QList>
#include "../backend/backend.h"

namespace Ui {
class ui_client;
}
class MainWindow;
class ui_client : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui_client(QWidget *parent = nullptr);
    ~ui_client();

    // 新增：设置当前用户信息
    void setCurrentUserId(int userId);
    void setCurrentUsername(const QString &username);

private slots:
    void on_pushButtonexit_clicked();

    void on_btnQueryById_clicked();
    void on_btnQueryByCode_clicked();
    void on_btnQueryAll_clicked();
    void on_editSearchCity_textChanged(const QString &text);

    // 机场查询相关
    void on_btnQueryAirportById_clicked();
    void on_btnQueryAirportByCode_clicked();
    void on_btnQueryAirportByCity_clicked();
    void on_btnQueryAllAirports_clicked();
    void on_editSearchAirport_textChanged(const QString &text);

    // 飞机查询相关
    void on_btnQueryAirplaneById_clicked();      // 按ID查询
    void on_btnQueryAllAirplanes_clicked();      // 显示所有飞机
    void on_editSearchAirplane_textChanged(const QString &text);  // 搜索

    //航班查询相关

    // 菜单导航
    void on_actionSearchFlight_triggered();  // 航班查询菜单项
    // 查询页面 (page_flight_search)
    void on_btnSearch_clicked();             // 查询按钮
    void on_btnSwapCities_clicked();         // 交换城市按钮
    // 列表页面 (page_flight_list)
    void on_btnBackFromList_clicked();       // 返回按钮
    // 详情页面 (page_flight_detail)
    void on_btnBackFromDetail_clicked();     // 返回按钮
    void on_btnBook_clicked();               // 预订按钮
    // 用户中心相关（新增）
    void on_actionUserCenter_triggered();  // 菜单：个人中心

    void on_btnConfirmBook_clicked();  //确认订票
    void on_radioEconomy_clicked();    // 选择经济舱
    void on_radioBusiness_clicked();   // 选择商务舱
    void on_radioFirst_clicked();      // 选择头等舱

    void on_btnRefreshTickets_clicked();                // 刷新
    void onRefundButtonClicked(int ticketId);           // 退票按钮
    void on_btnExit_clicked();
    void on_btnDeleteAccount_clicked();  // 注销账号按钮

private:
    Ui::ui_client *ui;
    MainWindow* mainUI=nullptr;

    // 用户信息
    int currentUserId = -1;
    QString currentUsername;

    void initCityQueryPage();
    void initAirportQueryPage();
    void initAirplaneQueryPage();
    void initFlightSearchPage();    // 初始化航班查询页面
    void initFlightListPage();      // 初始化航班列表页面
    void initFlightDetailPage();    // 初始化航班详情页面
    void displayCities(const QList<City> &cities);
    void displayAirports(const QList<Airport> &airports);
    void displayAirplanes(const QList<Airplane> &airplanes);
    // 数据缓存
    QList<City> currentCityList;
    // 当前显示的机场列表（用于搜索过滤）- 新增
    QList<Airport> currentAirportList;
    QList<Airplane> currentAirplaneList;
    //航班查询相关
    void showFlightListPage();
    void displayFlightCards();                            // 显示航班卡片
    void displayFlightDetail(int flightId);               // 显示航班详情
    QGroupBox* createFlightCard(const FlightDetailInfo& flight); // 创建航班卡片
    void fillTicketTable(const QMap<QString, TicketInfo>& tickets); // 填充机票表格
    QString formatDuration(const QDateTime& start, const QDateTime& end); // 格式化时长
    void loadCitiesToComboBox();                         // 加载城市到下拉框
    void clearFlightCards();                             // 清空航班卡片

    //航班查询数据缓存
    QList<FlightDetailInfo> currentFlightList;           // 当前查询到的航班列表
    int selectedFlightId;                                // 当前选中的航班ID

    // 查询条件
    QString currentFromCityCode;
    QString currentToCityCode;
    QDate currentDepartDate;

    void loadUserInfo();
    void updateUserInfoDisplay();

    // 新增：订票相关成员
    int currentTicketId = -1;           // 当前选中的票ID
    QString currentTicketClass;         // 当前选中的舱位等级
    double currentTicketPrice = 0.0;    // 当前选中的价格

    // 新增：辅助方法
    void updateSeatSelectionUI();       // 更新舱位选择UI状态
    void resetBookingInfo();           // 重置订票信息
    bool validateBooking();            // 验证订票条件

    // 新增：用户机票相关（简化版）
    void initUserTicketsPage();                          // 初始化页面
    void loadUserTickets();                             // 加载用户机票
    void clearTicketCards();                            // 清空卡片
    QGroupBox* createTicketCard(const Ticket& ticket, const FlightDetailInfo& flight);
    QString translateStatus(const QString& status);      // 翻译状态
    QString getStatusColor(const QString& status);       // 获取状态颜色
    bool canRefundTicket(const QString& status);         // 判断是否可退票
    void showNoTicketMessage(const QString& message);
    void moveToCenter();
    void showDeleteAccountDialog();      // 显示注销确认对话框
};

#endif // UI_CLIENT_H
