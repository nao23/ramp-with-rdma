#pragma once

#include <iostream>
#include "spdlog/spdlog.h"
#include "Communicator.h"
#include "Table.h"


class TrxExecutor {

protected:
    std::shared_ptr<spdlog::logger> logger;
    int id;

public:
    Communicator* com;
    Table* table;
 
    TrxExecutor(int id, Communicator* com, Table* table) : id(id), com(com), table(table) {}
    virtual ~TrxExecutor() = default;
    virtual void run() = 0;
};
