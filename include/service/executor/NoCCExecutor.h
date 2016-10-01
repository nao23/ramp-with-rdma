#pragma once

#include "TrxExecutor.h"
#include "Item.h"
#include "MessageHeader.h"


class NoCCExecutor : public TrxExecutor {

private:
    void prepare(const Item& item);
    void commit(const Timestamp& ts_c);
    void get(const Key& key);

public:    
    NoCCExecutor(int id, Communicator* com, Table* table) : TrxExecutor(id, com, table) {}
    ~NoCCExecutor() = default;
    void run();
};
