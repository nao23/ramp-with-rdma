#pragma once

#include <rdma/rdma_cma.h>
#include <rdma/rdma_verbs.h>
#include <boost/circular_buffer.hpp>
#include "Communicator.h"
#include "Buffer.h"
#include "HostAndPort.h"


class SendRecvSocket : public Communicator {

private:
    static std::shared_ptr<spdlog::logger> class_logger;  // for this class
    Buffer msg_buf;

protected:
    struct rdma_cm_id* client_id;
    Buffer verbs_buf;
    struct ibv_mr* verbs_mr;
    boost::circular_buffer<Buffer> send_bufs;

    void setup_verbs_buf();
    int poll_send_cq(int num_entries, struct ibv_wc* wc);
    Buffer get_send_buf();
    void post_send(const Buffer& buf);
    
    int poll_recv_cq(int num_entries, struct ibv_wc* wc);
    Buffer get_recv_buf();
    void post_recv(const Buffer& buf);
    
public:
    SendRecvSocket(struct rdma_cm_id* client_id);
    ~SendRecvSocket();
    static SendRecvSocket* connect(const HostAndPort& hp);

    void send_msg(MessageHeader header, char* body);
    void recv_header(MessageHeader* header);
    char* get_body(size_t body_size);
    void clear_msg_buf();
    void send_close();
};
