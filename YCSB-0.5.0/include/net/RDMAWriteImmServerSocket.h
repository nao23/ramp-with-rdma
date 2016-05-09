#pragma once

#include "RDMACMServerSocket.h"
#include "RDMAWriteImmSocket.h"


class RDMAWriteImmServerSocket : public Acceptor {
    
private:
    RDMACMServerSocket rsock;
    
public:
    RDMAWriteImmServerSocket(char* port_str) : rsock(port_str) {}
    ~RDMAWriteImmServerSocket() = default;
    Communicator* accept();
};

