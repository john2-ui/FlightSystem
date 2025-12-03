#ifndef USER_H
#define USER_H

#include <QString>
#include <QVector>

class User {
public:
    User() = default;

    User(int id,
         const QString& username,
         const QString& password,
         const QVector<int>& ticketsID)
        : m_id(id),
        m_username(username),
        m_password(password),
        m_ticketsID(ticketsID) {}

    User(const QString& username,
         const QString& password,
         const QVector<int>& ticketsID)
        : m_id(-1),
        m_username(username),
        m_password(password),
        m_ticketsID(ticketsID) {}

    // Getters
    int id() const { return m_id; }
    QString username() const { return m_username; }
    QString password() const { return m_password; }
    QVector<int> ticketsID() const { return m_ticketsID; }

    // Setters
    void setId(int id) { m_id = id; }
    void setUsername(const QString& username) { m_username = username; }
    void setPassword(const QString& password) { m_password = password; }
    void setTicketsID(const QVector<int>& ticketsID) { m_ticketsID = ticketsID; }


        // 将 ticketsID 序列化为 "1,3,5,7"
    QString ticketsIDToString() const {
        QStringList list;
        for (int id : m_ticketsID) {
            list << QString::number(id);
        }
        return list.join(",");
    }

    // 从 "1,3,5" 转为 QVector<int>
    static QVector<int> parseTicketsID(const QString& str) {
        QVector<int> result;
        for (const QString& s : str.split(",", Qt::SkipEmptyParts)) {
            result.push_back(s.toInt());
        }
        return result;
    }
private:
    int m_id;
    QString m_username;
    QString m_password;
    QVector<int> m_ticketsID;
};

#endif // USER_H
