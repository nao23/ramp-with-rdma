#pragma once

#include <string>
#include "Acceptor.h"
#include "TCPSocket.h"


class TCPServerSocket : public Acceptor {

private:
    int sock;

public:
    TCPServerSocket(char* port_str);
    ~TCPServerSocket();
    Communicator* accept();
};
