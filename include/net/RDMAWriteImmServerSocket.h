#pragma once

#include "Acceptor.h"
#include "RDMAWriteImmSocket.h"


class RDMAWriteImmServerSocket : public Acceptor {
    
private:
    struct rdma_cm_id* listen_id;
    
public:
    RDMAWriteImmServerSocket(char* port_str);
    ~RDMAWriteImmServerSocket() = default;
    Communicator* accept();
};

