#pragma once

#include "Acceptor.h"
#include "RDMAWriteSocket.h"


class RDMAWriteServerSocket : public Acceptor {
    
private:
    struct rdma_cm_id* listen_id;
    
public:
    RDMAWriteServerSocket(char* port_str);
    ~RDMAWriteServerSocket() = default;
    Communicator* accept();
};

