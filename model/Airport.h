#ifndef AIRPORT_H
#define AIRPORT_H

#include <QString>

class Airport {
public:
    Airport() = default;
    Airport(int id, const QString& name, const QString& code, int cityId, int terminalCount)
        : m_id(id), m_name(name), m_code(code), m_cityId(cityId), m_terminalCount(terminalCount) {}

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString code() const { return m_code; }
    void setCode(const QString& code) { m_code = code; }

    int cityId() const { return m_cityId; }
    void setCityId(int cityId) { m_cityId = cityId; }

    int terminalCount() const { return m_terminalCount; }
    void setTerminalCount(int count) { m_terminalCount = count; }

private:
    int m_id;
    QString m_name;
    QString m_code;
    int m_cityId;
    int m_terminalCount;
};

#endif // AIRPORT_H
