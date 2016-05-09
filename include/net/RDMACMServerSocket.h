#pragma once

#include "Acceptor.h"
#include "RDMACMSocket.h"

#define PACKET_WINDOW_SIZE (10)
#define PACKET_SIZE (2560)


class RDMACMServerSocket : boost::noncopyable {
    
private:
    struct rdma_cm_id* listen_id;

public:
    RDMACMServerSocket(char* port_str);
    ~RDMACMServerSocket();
    RDMACMSocket* accept();
};
