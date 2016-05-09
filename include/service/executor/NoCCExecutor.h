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
    NoCCExecutor(Communicator* com, Table* table) : TrxExecutor(com, table) {}
    ~NoCCExecutor() = default;
    void run();
};
