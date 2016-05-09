#pragma once

#include "ConnectionPool.h"
#include "Transaction.h"


class TrxHandler {

public:
    ConnectionPool cp;
    Transaction* trx;

    virtual ~TrxHandler() = default;
    virtual void put_all() = 0;
    virtual void get_all() = 0;

    template<class... Args> void send_to_all(MessageType msg_type, Args... args) {
	tbb::parallel_for_each(this->cp.access_coms,
	[&](Communicator* com) {			   
	    com->send(msg_type, args...);
	    com->recv();
	});
    }
};
