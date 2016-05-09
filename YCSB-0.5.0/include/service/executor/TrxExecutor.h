#pragma once

#include <iostream>
#include "Communicator.h"
#include "Table.h"


class TrxExecutor {

public:
    Communicator* com;
    Table* table;
 
    TrxExecutor(Communicator* com, Table* table) : com(com), table(table) {}
    virtual ~TrxExecutor() = default;
    virtual void run() = 0;
};
