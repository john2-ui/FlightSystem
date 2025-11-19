#ifndef TICKETDAO_H
#define TICKETDAO_H

#include "../model/Ticket.h"
#include <QList>

class TicketDao {
public:
    virtual ~TicketDao() = default;
    virtual bool insert(const Ticket& ticket) = 0;
    virtual bool update(const Ticket& ticket) = 0;
    virtual bool remove(int id) = 0;
    virtual Ticket getById(int id) = 0;
    virtual QList<Ticket> getAll() = 0;
};

#endif
