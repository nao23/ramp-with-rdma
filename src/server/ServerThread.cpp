#include "ServerThread.h"


ServerThread::ServerThread(TrxExecutor* trx_executor) {
    this->trx_executor = trx_executor;
}

void ServerThread::start() {
    std::thread th(&ServerThread::run, this);
    th.detach();
}

void ServerThread::run() {
    this->trx_executor->run();
    delete this->trx_executor;
}
