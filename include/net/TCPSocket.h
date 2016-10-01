#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include "HostAndPort.h"
#include "Communicator.h"
#include "Buffer.h"


class TCPSocket : public Communicator {

private:
    static std::shared_ptr<spdlog::logger> class_logger;  // for this class    
    int sock;
    Buffer msg_buf;

    void send(void* buf, size_t count) const;
    template <class T> void send(T* data) const {
	send(data, sizeof(T));
    }

    void recv(void* buf, size_t count) const;
    template <class T> void recv(T* data) const {
	recv(data, sizeof(T));
    }

public:
    TCPSocket(int sock);
    ~TCPSocket();
    static TCPSocket* connect(const HostAndPort& hp);
    
    void send_msg(MessageHeader header, char* body);
    void recv_header(MessageHeader* header);
    char* get_body(size_t body_size);
    void clear_msg_buf();
    void send_close();
};
