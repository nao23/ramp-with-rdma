#pragma once

#include <boost/range/adaptor/map.hpp>
#include "Containers.h"
#include "Communicator.h"
#include "TCPSocket.h"
#include "SendRecvSocket.h"
#include "RDMAWriteSocket.h"
#include "RDMAWriteImmSocket.h"


class ConnectionPool {

public:
    ConcurrentMap<HostAndPort, Communicator*> connections;
    ConcurrentSet<Communicator*> access_coms;

    ConnectionPool() = default;   
    void connect_all();
    Communicator* get(const Key& key);
    void clear_access_coms();
    void close_all();
};
