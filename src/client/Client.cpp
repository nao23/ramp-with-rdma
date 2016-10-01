#include "Client.h"


Client::Client() {

    this->logger = spdlog::stdout_logger_mt("Client", true);

    Config& config = Config::get_config();
    this->trx_queue.gen_test_trxs();
    
    for (int i = 0; i < config.worker_num; ++i) {
	
	TrxHandler* trx_handler;
	if (config.trx_type == TrxType::LOCK_BASED) {
	    trx_handler = new LockBasedHandler(i);
	} else if (config.trx_type == TrxType::NO_CONCURRENCY_CONTROL) { 
	    trx_handler = new NoCCHandler(i);
	} else if (config.trx_type == TrxType::RAMP_FAST) {
	    trx_handler = new RAMPFastHandler(i);
	} else if (config.trx_type == TrxType::AC_RAMP_FAST) { 
	    trx_handler = new ACRAMPFastHandler(i);
	} else {
	    break;
	}

	this->workers.push_back(ClientThread(&this->trx_queue, trx_handler));
	this->handlers.push_back(trx_handler);
    }

}

void Client::start_all() {
    
    this->logger->info("Connect to all servers");    
    for (auto& handler : this->handlers) {
	handler->cp.connect_all();
    }

    this->logger->info("Start all worker threads");
    this->bgn = Timestamp::now();
    for (auto &w : this->workers) {
	w.start();
    }
}

void Client::check_finish() {

    for (auto &w : this->workers) {
	w.join();
    }
    this->logger->info("All worker thread joined");

    this->end = Timestamp::now();

    this->logger->info("Close all connections");
    for (auto& handler : this->handlers) {
	handler->cp.close_all();
	delete handler;
    }
}

void Client::print_result() const {

    Config& config = Config::get_config();
    double run_time = Timestamp::get_elapsed_sec(this->bgn, this->end);
    
    this->logger->info("*** performance result ***");
    this->logger->info("RunTime(sec): {}", run_time);
    this->logger->info("Throughput(trx/sec): {}", ((config.read_trx_num + config.write_trx_num)/ run_time));
}
