#pragma once

#include <thread>
#include "TrxExecutor.h"


class ServerThread {

private:
    void run();

public:
    TrxExecutor* trx_executor;
    
    ServerThread(TrxExecutor* trx_dispatcher);
    void start();
};
