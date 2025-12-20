#include "DBManager.h"
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QSqlError>
#include <QCoreApplication>

DBManager &DBManager::instance() {
  static DBManager instance;
  return instance;
}

DBManager::DBManager() {
  // 配置文件路径：程序可执行文件所在目录/config/db.ini
    QString configPath = QDir::cleanPath(QDir::currentPath() + "/../../../../config/db.ini");

  QSettings settings(configPath, QSettings::IniFormat);

  QString host = settings.value("database/host", "127.0.0.1").toString();
  int port = settings.value("database/port", 3306).toInt();
  QString user = settings.value("database/user", "root").toString();
  QString password = settings.value("database/password", "").toString();
  QString database = settings.value("database/database", "").toString();

  qDebug() << "Config path:" << configPath;
  qDebug() << "host:" << host
           << "port:" << port
           << "user:" << user
           << "password:" << password
           << "database:" << database;


 m_db = QSqlDatabase::addDatabase("QMYSQL");
  // m_db = QSqlDatabase::addDatabase("QODBC");
  m_db.setHostName(host);
  m_db.setPort(port);
  m_db.setUserName(user);
  m_db.setPassword(password);
  m_db.setDatabaseName(database);


  if (!m_db.open()) {
    qDebug() << "数据库连接失败:" << m_db.lastError().text() ;
      // qDebug() << host << port << user << password << database;
  } else {
    qDebug() << "数据库连接成功";
  }
}

QSqlDatabase &DBManager::db() { return m_db; }

void DBManager::close() {
  if (m_db.isOpen())
    m_db.close();
}
