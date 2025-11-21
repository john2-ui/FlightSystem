#ifndef TICKET_DAO_IMPL_H
#define TICKET_DAO_IMPL_H

#include "../dao/TicketDao.h"
#include <QSqlDatabase>

class TicketDaoImpl : public TicketDao {
public:
    TicketDaoImpl();
    int insert(const Ticket& ticket) override;
    bool update(const Ticket& ticket) override;
    bool remove(int id) override;
    Ticket getById(int id) override;
    QList<Ticket> getAll() override;

private:
    QSqlDatabase m_db;
};

#endif // TICKET_DAO_IMPL_H
