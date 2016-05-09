#pragma once

#include "TrxExecutor.h"
#include "Item.h"
#include "MessageHeader.h"


class RAMPFastExecutor : public TrxExecutor {

protected:
    void prepare(const Item& item);
    void commit(const Timestamp& ts_c);
    void get(const Key& key, const Timestamp& ts_req);

public:
    RAMPFastExecutor(Communicator* com, Table* table) : TrxExecutor(com, table) {}
    ~RAMPFastExecutor() = default;
    void run();
};
