#pragma once

#include <stack>
#include "spdlog/spdlog.h"
#include "Config.h"
#include "Acceptor.h"
#include "Table.h"
#include "ServerThread.h"
#include "LockBasedExecutor.h"
#include "NoCCExecutor.h"
#include "RAMPFastExecutor.h"
#include "ACRAMPFastExecutor.h"


class Server {

private:
    std::shared_ptr<spdlog::logger> logger;
    Acceptor* acc;
    Table table;
    LockManager lock_manager;

public:
    Server(Acceptor* acc);
    ~Server();
    void start();
};
