#include "RDMAWriteSocket.h"


std::shared_ptr<spdlog::logger> RDMAWriteSocket::class_logger = spdlog::stdout_logger_mt("RDMAWriteSocket", true);

RDMAWriteSocket::RDMAWriteSocket(struct rdma_cm_id* client_id) : SendRecvSocket(client_id) {
    
    this->write_mr = NULL;
    setup_write_buf();

    // send rkey and addr
    Buffer send_buf = this->get_send_buf();
    RemoteKeyAndAddr rka(this->write_mr->rkey, this->write_buf.addr);
    send_buf.write(rka);
    this->post_send(send_buf);
    
    // receive rkey and addr
    Buffer recv_buf = this->get_recv_buf();
    recv_buf.read(&this->rka);
    this->post_recv(recv_buf);
}

RDMAWriteSocket::~RDMAWriteSocket() {
    rdma_dereg_mr(this->write_mr);
    this->write_buf.free();
}

RDMAWriteSocket* RDMAWriteSocket::connect(const HostAndPort& hp) {

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

    return new RDMAWriteSocket(client_id);
}

void RDMAWriteSocket::setup_write_buf() {

    this->write_buf = Buffer::allocate(PACKET_SIZE);
    
    this->write_mr = rdma_reg_write(this->client_id, this->write_buf.addr, this->write_buf.size);
    if (this->write_mr == NULL) {
	this->logger->error("rdma_reg_write: {}", strerror(errno));
	this->write_buf.free();
        exit(EXIT_FAILURE);
    }
}

void RDMAWriteSocket::post_write(const Buffer& buf, const RemoteKeyAndAddr& rka) {

    uint32_t rkey = rka.rkey;
    uint64_t raddr = rka.remote_addr;

    if (rdma_post_write(this->client_id, buf.addr, buf.addr, buf.size, this->verbs_mr, 0, raddr, rkey) < 0) {
	this->logger->error("rdma_post_write: {}", strerror(errno));
	this->send_bufs.push_front(buf);
        exit(EXIT_FAILURE);
    }
}

void RDMAWriteSocket::post_read(const Buffer& buf, const RemoteKeyAndAddr& rka) {

    uint32_t rkey = rka.rkey;
    uint64_t raddr = rka.remote_addr;

    if (rdma_post_read(this->client_id, buf.addr, buf.addr, buf.size, this->verbs_mr, 0, raddr, rkey) < 0) {
	this->logger->error("rdma_post_read: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void RDMAWriteSocket::setup_read_buf() { // only called on server side
    
    Config& config = Config::get_config();

    this->read_mr = rdma_reg_read(this->client_id, config.cache_addr, config.cache_size);
    if (this->read_mr == NULL) {
	this->logger->error("rdma_reg_read: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int RDMAWriteSocket::read(Item* item, RemoteKeyAndAddr rka) {
    
    Buffer read_buf = this->get_send_buf();

    this->post_read(read_buf, rka);
    
    printf("read invalid flag + MessageHeader..."); fflush(stdout);
    // read invalid flag + MessageHeader
    int is_invalid;
    MessageHeader header;
    volatile int* is_arrived = (int*)(read_buf.addr + sizeof(int) + offsetof(MessageHeader, is_arrived));
    while (true) {
	if (*is_arrived != 0) {
	    read_buf.read(&is_invalid).read(&header);
	    break;
	}
    }
    printf("done\n"); fflush(stdout);

    if (is_invalid != 0) {
	return -1;
    }
    
    printf("read body + is_arrived flag..."); fflush(stdout);
    // read body + is_arrived flag
    is_arrived = (int*)(read_buf.addr + sizeof(int) + sizeof(MessageHeader) + header.body_size);
    while (true) {
	if (*is_arrived != 0) {
	    break;
	}
    }
    printf("done\n"); fflush(stdout);

    char* body = read_buf.addr + sizeof(int) + sizeof(MessageHeader);

    try {
	// deserialize
	msgpack::unpacked res;
	msgpack::unpack(&res, body, header.body_size);
	msgpack::object obj = res.get();
	obj.convert(item);
    } catch (msgpack::v1::insufficient_bytes e) {
	return -1;
    } catch (msgpack::v1::type_error e) {
	return -1;
    }
    
    return 0;
}

void RDMAWriteSocket::send_msg(MessageHeader header, char* body) {

    Buffer send_buf = this->get_send_buf();
    int is_arrived = 0xffffffff;
    send_buf.write(header).write(body, header.body_size).write(is_arrived);
    this->post_write(send_buf, this->rka);
}

void RDMAWriteSocket::recv_header(MessageHeader* header) {

    volatile int* is_arrived = (int*)(this->write_buf.addr + offsetof(MessageHeader, is_arrived));
    while (true) {
	if (*is_arrived != 0) {
	    this->write_buf.read(header);
	    return;
	} 
    }
}

char* RDMAWriteSocket::get_body(size_t body_size) {

    volatile int* is_arrived = (int*)(this->write_buf.addr + sizeof(MessageHeader) + body_size);
    while (true) {
	if (*is_arrived != 0) {
	    return (this->write_buf.addr + sizeof(MessageHeader));
	} 
    }
}

void RDMAWriteSocket::clear_msg_buf() {
    this->write_buf.clear();
}

void RDMAWriteSocket::send_close() {

    Buffer send_buf = this->get_send_buf();

    // clear send cq
    struct ibv_wc wc[PACKET_WINDOW_SIZE];
    //this->poll_send_cq(PACKET_WINDOW_SIZE, wc);
    
    int ret = ibv_poll_cq(this->client_id->send_cq, PACKET_WINDOW_SIZE, wc);
    if (ret < 0) {
	this->logger->error("ibv_poll_cq: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    // send close msg
    MessageHeader header(MessageType::CLOSE, 0);
    int is_arrived = 0xffffffff;
    send_buf.write(header).write(is_arrived);
    this->post_write(send_buf, this->rka);

    // check send
    struct ibv_wc close_wc;
    this->poll_send_cq(1, &close_wc);
}
