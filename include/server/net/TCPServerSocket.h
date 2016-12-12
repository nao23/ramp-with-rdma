#pragma once

#include "Acceptor.h"
#include "TCPSocket.h"


class TCPServerSocket : public Acceptor {

private:
    int sock;

public:
    TCPServerSocket(std::string port_str);
    ~TCPServerSocket();
    Communicator* accept();
};
