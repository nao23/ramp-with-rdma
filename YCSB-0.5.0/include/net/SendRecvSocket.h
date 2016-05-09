#pragma once

#include "RDMACMSocket.h"
#include "Communicator.h"


class SendRecvSocket : public Communicator {

private:
    Buffer msg_buf;
    
public:
    RDMACMSocket* rsock;

    SendRecvSocket(RDMACMSocket* rsock);
    ~SendRecvSocket();
    static SendRecvSocket* connect(const HostAndPort& hp);

    void send_msg(MessageHeader header, char* body);
    void recv_header(MessageHeader* header);
    char* get_body(size_t body_size);
    void clear_msg_buf();
    void send_close();
};
