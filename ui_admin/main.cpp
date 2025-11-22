#include "ui_admin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ui_admin w;
    w.show();
    return a.exec();
}
