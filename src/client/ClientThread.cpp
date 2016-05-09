#include "ClientThread.h"


ClientThread::ClientThread(TrxQueue* trx_queue, TrxHandler* trx_handler) {
    this->trx_queue = trx_queue;
    this->trx_handler = trx_handler;
}

void ClientThread::start() {
    this->th = std::thread(&ClientThread::run, this);
}

void ClientThread::run() {
    
    while (this->trx_queue->pop(this->trx_handler->trx)) {
	if (!this->trx_handler->trx->write_set.empty()) {
	    this->trx_handler->put_all();
	}
	if (!this->trx_handler->trx->read_set.empty()) {
	    this->trx_handler->get_all();
	}
	this->trx_handler->cp.clear_access_coms();
    }
}

void ClientThread::join() {
    this->th.join();
}
