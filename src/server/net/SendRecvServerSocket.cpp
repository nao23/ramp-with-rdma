#include "SendRecvServerSocket.h"


SendRecvServerSocket::SendRecvServerSocket(std::string port_str) {

    this->logger = spdlog::stdout_logger_mt("SendRecvServerSocket", true);
    
    this->listen_id = NULL;
    
    struct rdma_addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = RAI_PASSIVE;
    hints.ai_port_space = RDMA_PS_TCP;
    res = NULL;

    if (rdma_getaddrinfo(NULL, const_cast<char*>(port_str.c_str()), &hints, &res) < 0) {
        this->logger->error("rdma_getaddrinfo: {}", strerror(errno));
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
    
    if (rdma_create_ep(&this->listen_id, res, NULL, &attr) < 0) {
        this->logger->error("rdma_create_ep: {}", strerror(errno));
	rdma_freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }
    
    rdma_freeaddrinfo(res);
    
    if (rdma_listen(this->listen_id, LISTEN_MAX) < 0) {
        this->logger->error("rdma_listen: {}", strerror(errno));
        rdma_destroy_ep(this->listen_id);
        exit(EXIT_FAILURE);
    }
}

Communicator* SendRecvServerSocket::accept() {

    struct rdma_cm_id* client_id = NULL;
    if (rdma_get_request(this->listen_id, &client_id) < 0) {
        this->logger->error("rdma_get_request: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (rdma_accept(client_id, NULL) < 0) {
        this->logger->error("rdma_accept: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return new SendRecvSocket(client_id);
}
