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
    ui_admin(QWidget *parent = nullptr);
    ~ui_admin();

private slots:
    void AddFlightClicked();
    void UpdateFlightClicked();
    void DeleteFlightClicked();
    void AddPlaneClicked();
    void UpdatePlaneClicked();
    void DeletePlaneClicked();

private:
    Ui::ui_admin *ui;
    QWidget* dynamicWidget;
};
#endif // UI_ADMIN_H
