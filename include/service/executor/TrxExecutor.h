#pragma once

#include <iostream>
#include "Communicator.h"
#include "Table.h"


class TrxExecutor {

protected:
    int id;

public:
    Communicator* com;
    Table* table;
 
    TrxExecutor(int id, Communicator* com, Table* table) : id(id), com(com), table(table) {}
    virtual ~TrxExecutor() = default;
    virtual void run() = 0;
};
