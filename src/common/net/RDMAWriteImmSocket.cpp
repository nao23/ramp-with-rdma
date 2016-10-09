#include "RDMAWriteImmSocket.h"


RDMAWriteImmSocket* RDMAWriteImmSocket::connect(const HostAndPort& hp) {

    struct rdma_cm_id* client_id = NULL; 

    struct rdma_addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_port_space = RDMA_PS_TCP;
    res = NULL;

    char* hostname = const_cast<char*>(hp.hostname);
    char* port_str = const_cast<char*>(hp.port_str);
    
    if (rdma_getaddrinfo(hostname, port_str, &hints, &res) < 0) {
	class_logger->error("rdma_getaddrinfo: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    struct ibv_qp_init_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.cap.max_send_wr = PACKET_WINDOW_SIZE; 
    attr.cap.max_recv_wr = PACKET_WINDOW_SIZE; 
    attr.cap.max_send_sge = 1;
    attr.cap.max_recv_sge = 1;
    attr.cap.max_inline_data = 0;
    attr.sq_sig_all = 1;
    
    if (rdma_create_ep(&client_id, res, NULL, &attr) < 0) {
	class_logger->error("rdma_create_ep: {}", strerror(errno));
	rdma_freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    rdma_freeaddrinfo(res);
    
    if (rdma_connect(client_id, NULL) < 0) {
	class_logger->error("rdma_connect: {}", strerror(errno));
        rdma_destroy_ep(client_id);
        exit(EXIT_FAILURE);
    }
    
    return new RDMAWriteImmSocket(client_id);
}

void RDMAWriteImmSocket::post_write_imm(const Buffer& buf, const RemoteKeyAndAddr& rka) {
    if (rdma_post_write_imm(this->client_id, buf.addr, buf.addr, buf.size, this->verbs_mr, 0, rka.remote_addr, rka.rkey) < 0) {
	this->logger->error("rdma_post_write_imm: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void RDMAWriteImmSocket::send_msg(MessageHeader header, char* body) {
    Buffer send_buf = this->get_send_buf();
    int is_arrived = 0xffffffff;
    send_buf.write(header).write(body, header.body_size).write(is_arrived);
    this->post_write_imm(send_buf, this->rka);
}

void RDMAWriteImmSocket::recv_header(MessageHeader* header) {
    this->msg_buf = this->get_recv_buf();
    this->write_buf.read(header);
}

char* RDMAWriteImmSocket::get_body(size_t body_size) {
    return (this->write_buf.addr + sizeof(MessageHeader));
}

void RDMAWriteImmSocket::clear_msg_buf() {
    this->write_buf.clear();
    this->post_recv(this->msg_buf);
}

void RDMAWriteImmSocket::send_close() {

    Buffer send_buf = this->get_send_buf();

    // clear send cq
    struct ibv_wc wc[PACKET_WINDOW_SIZE];
    int ret;
    ret = ibv_poll_cq(this->client_id->send_cq, PACKET_WINDOW_SIZE, wc);
    if (ret < 0) {
	this->logger->error("ibv_poll_cq: {}", strerror(errno));
        exit(EXIT_FAILURE); 
    }
    
    // send close msg
    MessageHeader header(MessageType::CLOSE, 0);
    int is_arrived = 0xffffffff;
    send_buf.write(header).write(is_arrived);
    this->post_write_imm(send_buf, this->rka);

    // check send
    struct ibv_wc close_wc;
    this->poll_send_cq(1, &close_wc);
}

std::shared_ptr<spdlog::logger> RDMAWriteImmSocket::class_logger = spdlog::stdout_logger_mt("RDMAWriteImmSocket", true);
