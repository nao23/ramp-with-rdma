#pragma once

#include "RDMACMServerSocket.h"
#include "SendRecvSocket.h"


class SendRecvServerSocket : public Acceptor {

private:
    RDMACMServerSocket rsock;

public:
    SendRecvServerSocket(char* port_str) : rsock(port_str) {}
    ~SendRecvServerSocket() = default;
    Communicator* accept();
};
