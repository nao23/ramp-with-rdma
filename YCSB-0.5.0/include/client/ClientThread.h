#pragma once

#include <thread>
#include "TrxQueue.h"
#include "TrxHandler.h"


class ClientThread {

private:
    TrxQueue* trx_queue;
    TrxHandler* trx_handler;
    std::thread th;

    void run();

public:
    ClientThread(TrxQueue* trx_queue, TrxHandler* trx_handler);
    
    void start();
    void join();
};
