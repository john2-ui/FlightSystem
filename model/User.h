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
         const QVector<int>& ticketsID,
        int isSuper = 0)
        : m_id(id),
        m_username(username),
        m_password(password),
        m_ticketsID(ticketsID),
        m_isSuper(isSuper) {}

    User(const QString& username,
         const QString& password,
         const QVector<int>& ticketsID,
        int isSuper = 0)
        : m_id(-1),
        m_username(username),
        m_password(password),
        m_ticketsID(ticketsID),
        m_isSuper(isSuper) {}

    // Getters
    int id() const { return m_id; }
    QString username() const { return m_username; }
    QString password() const { return m_password; }
    QVector<int> ticketsID() const { return m_ticketsID; }
    int isSuper() const { return m_isSuper; }

    // Setters
    void setId(int id) { m_id = id; }
    void setUsername(const QString& username) { m_username = username; }
    void setPassword(const QString& password) { m_password = password; }
    void setTicketsID(const QVector<int>& ticketsID) { m_ticketsID = ticketsID; }
    void setIsSuper(int isSuper) { m_isSuper = isSuper; }


        // 将 ticketsID 序列化为 "1,3,5,7"
    QString ticketsIDToString() const {
        if(m_ticketsID.isEmpty()){
            return "-1";
        }
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
    int m_isSuper;  // 0=普通用户 1=管理员
};

#endif // USER_H
