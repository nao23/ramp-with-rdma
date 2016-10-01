#pragma once

#include "spdlog/spdlog.h"
#include "Config.h"
#include "ClientThread.h"
#include "LockBasedHandler.h"
#include "NoCCHandler.h"
#include "RAMPFastHandler.h"
#include "ACRAMPFastHandler.h"


class Client {

private:
    std::shared_ptr<spdlog::logger> logger;
    Timestamp bgn, end;
    TrxQueue trx_queue;
    Vector<ClientThread> workers;
    Vector<TrxHandler*> handlers;

public:
    Client();
    void start_all();
    void check_finish();
    void print_result() const;
};
