#pragma once

#include "RDMACMServerSocket.h"
#include "RDMAWriteSocket.h"


class RDMAWriteServerSocket : public Acceptor {
    
private:
    RDMACMServerSocket rsock;
    
public:
    RDMAWriteServerSocket(char* port_str) : rsock(port_str) {}
    ~RDMAWriteServerSocket() = default;
    Communicator* accept();
};

