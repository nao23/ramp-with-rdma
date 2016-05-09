#include "Client.h"


Client::Client() {

    Config& config = Config::get_config();
    this->trx_queue.gen_test_trxs();
    
    for (int i = 0; i < config.worker_num; ++i) {
	
	TrxHandler* trx_handler;
	if (config.trx_type == TrxType::LOCK_BASED) {
	    trx_handler = new LockBasedHandler();
	} else if (config.trx_type == TrxType::NO_CONCURRENCY_CONTROL) { 
	    trx_handler = new NoCCHandler();
	} else if (config.trx_type == TrxType::RAMP_FAST) {
	    trx_handler = new RAMPFastHandler();
	} else if (config.trx_type == TrxType::AC_RAMP_FAST) { 
	    trx_handler = new ACRAMPFastHandler();
	} else {
	    break;
	}

	this->workers.push_back(ClientThread(&this->trx_queue, trx_handler));
	this->handlers.push_back(trx_handler);
    }

}

void Client::start_all() {

    for (auto& handler : this->handlers) {
	handler->cp.connect_all();
    }

    this->bgn = Timestamp::now();

    for (auto &w : this->workers) {
	w.start();
    }
}

void Client::check_finish() {

    for (auto &w : this->workers) {
	w.join();
    }

    this->end = Timestamp::now();

    for (auto& handler : this->handlers) {
	handler->cp.close_all();
	delete handler;
    }
}

void Client::print_result() const {

    Config& config = Config::get_config();
    double run_time = Timestamp::get_elapsed_sec(this->bgn, this->end);
    
    //std::cout << "RunTime(sec): " << std::setprecision(4) << run_time << std::endl;
    //std::cout << "Throughput(trx/sec): " << std::setprecision(4) << (config.trx_num / run_time) << std::endl;
    printf("RunTime(sec): %lf\n", run_time);
    printf("Throughput(trx/sec): %lf\n", (config.trx_num / run_time));
}
