#pragma once

#include "RAMPFastExecutor.h"
#include "RDMAWriteSocket.h"


class ACRAMPFastExecutor : public RAMPFastExecutor {

private:
    RDMAWriteSocket* rdma_com;
    void get_item_and_addr(Key key, Timestamp ts_req);

public:
    void run();
    ACRAMPFastExecutor(Communicator* com, Table* table);
    ~ACRAMPFastExecutor() = default;
};
