#pragma once

#include <rdma/rdma_cma.h>
#include <rdma/rdma_verbs.h>
#include <boost/circular_buffer.hpp>
#include "Communicator.h"
#include "SendRecvSocket.h"
#include "Buffer.h"
#include "HostAndPort.h"
#include "RemoteKeyAndAddr.h"
#include "Config.h"
#include "Item.h"


class RDMAWriteSocket : public SendRecvSocket {

private:       
    static std::shared_ptr<spdlog::logger> class_logger;  // for this class

    void setup_write_buf();

protected:
    void post_write(const Buffer& buf, const RemoteKeyAndAddr& rka);
    void post_read(const Buffer& buf, const RemoteKeyAndAddr& rka);
    
public:
    struct ibv_mr* read_mr;
    Buffer write_buf;
    struct ibv_mr* write_mr;
    RemoteKeyAndAddr rka;

    RDMAWriteSocket(struct rdma_cm_id* client_id);
    ~RDMAWriteSocket();
    static RDMAWriteSocket* connect(const HostAndPort& hp);

    void setup_read_buf();
    int read(Item* item, RemoteKeyAndAddr rka);
    
    void send_msg(MessageHeader header, char* body);
    void recv_header(MessageHeader* header);
    char* get_body(size_t body_size);
    void clear_msg_buf();
    void send_close();
};
