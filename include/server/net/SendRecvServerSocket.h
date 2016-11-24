#pragma once

#include "Acceptor.h"
#include "SendRecvSocket.h"


class SendRecvServerSocket : public Acceptor {

private:
    struct rdma_cm_id* listen_id;

public:
    SendRecvServerSocket(std::string port_str);
    ~SendRecvServerSocket() = default;
    Communicator* accept();
};
