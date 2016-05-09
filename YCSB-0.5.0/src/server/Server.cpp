#include "Server.h"


Server::Server(Acceptor* acc) { 
    this->acc = acc;
}

Server::~Server() { 
    delete this->acc;
}

void Server::start() {

    Config& config = Config::get_config();

    config.cache_addr = this->table.committed_items.addr;
    config.cache_size = this->table.committed_items.size;

    int i = 0;
    Vector<ServerThread> sts;
    sts.reserve(1000);

    while (true) {
	
	TrxExecutor* trx_executor;
				
	if (config.trx_type == TrxType::LOCK_BASED) {
	    trx_executor = new LockBasedExecutor(this->acc->accept(), &this->table, &this->lock_manager);   
	} else if (config.trx_type == TrxType::NO_CONCURRENCY_CONTROL) {
	    trx_executor = new NoCCExecutor(this->acc->accept(), &this->table);
	} else if (config.trx_type == TrxType::RAMP_FAST) {
	    trx_executor = new RAMPFastExecutor(this->acc->accept(), &this->table);
	} else if (config.trx_type == TrxType::AC_RAMP_FAST) { 
	    trx_executor = new ACRAMPFastExecutor(this->acc->accept(), &this->table);
	} else {
	    break;
	}
	
        sts.push_back(ServerThread(trx_executor));
        sts[i++].start();
    }
} 
