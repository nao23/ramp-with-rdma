#pragma once

#include "Acceptor.h"
#include "RDMAWriteSocket.h"


class RDMAWriteServerSocket : public Acceptor {
    
private:
    struct rdma_cm_id* listen_id;
    
public:
    RDMAWriteServerSocket(std::string port_str);
    ~RDMAWriteServerSocket() = default;
    Communicator* accept();
};

