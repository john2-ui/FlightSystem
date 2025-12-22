#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QMainWindow>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>
#include <QList>
#include "../backend/backend.h"

#include <QTableView>
#include <QStandardItemModel>

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
    void setCurrentUserId(int userId);
    void setCurrentUsername(const QString &username);

private slots:
    void on_pushButtonexit_clicked();
    //城市查询
    void on_btnQueryById_clicked();
    void on_btnQueryByCode_clicked();
    void on_btnQueryAll_clicked();
    void on_editSearchCity_textChanged(const QString &text);
    // 机场查询
    void on_btnQueryAirportById_clicked();
    void on_btnQueryAirportByCode_clicked();
    void on_btnQueryAirportByCity_clicked();
    void on_btnQueryAllAirports_clicked();
    void on_editSearchAirport_textChanged(const QString &text);
    // 飞机查询
    void on_btnQueryAirplaneById_clicked();
    void on_btnQueryAllAirplanes_clicked();
    void on_editSearchAirplane_textChanged(const QString &text);
    //航班查询
    void on_actionSearchFlight_triggered();
    void on_btnSearch_clicked();
    void on_btnSwapCities_clicked();
    void on_btnBackFromList_clicked();
    void on_btnBackFromDetail_clicked();
    void on_btnBook_clicked();
    // 用户中心
    void on_actionUserCenter_triggered();
    void on_btnConfirmBook_clicked();
    void on_radioEconomy_clicked();
    void on_radioBusiness_clicked();
    void on_radioFirst_clicked();
    void on_btnRefreshTickets_clicked();
    void onRefundButtonClicked(int ticketId);
    void on_btnExit_clicked();
    void on_btnDeleteAccount_clicked();

    void on_btnallflight_clicked();
    void onFlightNoClicked(const QModelIndex& index);

private:
    Ui::ui_client *ui;
    MainWindow* mainUI=nullptr;
    int currentUserId = -1;
    QString currentUsername;

    void initCityQueryPage();
    void initAirportQueryPage();
    void initAirplaneQueryPage();
    void initFlightSearchPage();
    void initFlightListPage();
    void initFlightDetailPage();
    void displayCities(const QList<City> &cities);
    void displayAirports(const QList<Airport> &airports);
    void displayAirplanes(const QList<Airplane> &airplanes);
    QList<City> currentCityList;
    QList<Airport> currentAirportList;
    QList<Airplane> currentAirplaneList;
    //航班查询
    void showFlightListPage();
    void displayFlightCards();
    void displayFlightDetail(int flightId);
    QGroupBox* createFlightCard(const FlightDetailInfo& flight);
    void fillTicketTable(const QMap<QString, TicketInfo>& tickets);
    QString formatDuration(const QDateTime& start, const QDateTime& end);
    void loadCitiesToComboBox();
    void clearFlightCards();
    QList<FlightDetailInfo> currentFlightList;
    int selectedFlightId;
    QString currentFromCityCode;
    QString currentToCityCode;
    QDate currentDepartDate;
    void loadUserInfo();
    void updateUserInfoDisplay();
    int currentTicketId = -1;
    QString currentTicketClass;
    double currentTicketPrice = 0.0;
    void updateSeatSelectionUI();
    void resetBookingInfo();
    bool validateBooking();
    // 用户机票
    void initUserTicketsPage();
    void loadUserTickets();
    void clearTicketCards();
    QGroupBox* createTicketCard(const Ticket& ticket, const FlightDetailInfo& flight);
    QString translateStatus(const QString& status);
    QString getStatusColor(const QString& status);
    bool canRefundTicket(const QString& status);
    void showNoTicketMessage(const QString& message);
    void moveToCenter();
    void showDeleteAccountDialog();

    QStandardItemModel *flightModel; // 新增：数据模型
    void loadAllFlightsToTable();
    void displayFlightsToTable(const QList<FlightDetailInfo>& flightList);
    void initFlightTableView();
    QString translateClass(const QString& flightclass);
};

#endif // UI_CLIENT_H
