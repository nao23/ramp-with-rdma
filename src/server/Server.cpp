#include "Server.h"


Server::Server(Acceptor* acc) {    
    this->logger = spdlog::stdout_logger_mt("Server", true);
    this->acc = acc;
}

Server::~Server() {
    delete this->acc;
}

void Server::start() {
    
    this->logger->info("Start");

    Config& config = Config::get_config();
    config.cache_addr = this->table.committed_items.addr;
    config.cache_size = this->table.committed_items.size;

    int id = 0;
    std::stack<ServerThread> server_threads;
    
    while (true) {

	TrxExecutor* trx_executor;
				
	if (config.trx_type == TrxType::TWO_PHASE_LOCKING) {
	    trx_executor = new LockBasedExecutor(id++, this->acc->accept(), &this->table, &this->lock_manager);   
	} else if (config.trx_type == TrxType::NO_CONCURRENCY_CONTROL) {
	    trx_executor = new NoCCExecutor(id++, this->acc->accept(), &this->table);
	} else if (config.trx_type == TrxType::RAMP_FAST) {
	    trx_executor = new RAMPFastExecutor(id++, this->acc->accept(), &this->table);
	} else if (config.trx_type == TrxType::AC_RAMP_FAST) { 
	    trx_executor = new ACRAMPFastExecutor(id++, this->acc->accept(), &this->table);
	} else {
	    break;
	}

	server_threads.push(ServerThread(trx_executor));
	server_threads.top().start();
    }
} 
