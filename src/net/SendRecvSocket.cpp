#include "SendRecvSocket.h"


SendRecvSocket::SendRecvSocket(RDMACMSocket* rsock) {
    this->rsock = rsock;
}

SendRecvSocket::~SendRecvSocket() {
    delete this->rsock;
}

SendRecvSocket* SendRecvSocket::connect(const HostAndPort& hp) {    
    return new SendRecvSocket(RDMACMSocket::connect(hp));
}

void SendRecvSocket::send_msg(MessageHeader header, char* body) {

    Buffer send_buf = this->rsock->get_send_buf();
    send_buf.write(header).write(body, header.body_size);
    this->rsock->post_send(send_buf);
}

void SendRecvSocket::recv_header(MessageHeader* header) {

    this->msg_buf = this->rsock->get_recv_buf();
    this->msg_buf.read(header);
}

char* SendRecvSocket::get_body(size_t body_size) {
    return (this->msg_buf.addr + sizeof(MessageHeader));
}

void SendRecvSocket::clear_msg_buf() {
    this->rsock->post_recv(this->msg_buf);
}

void SendRecvSocket::send_close() {

    Buffer send_buf = this->rsock->get_send_buf();

    // clear send cq
    struct ibv_wc wc[PACKET_WINDOW_SIZE];

    int ret = ibv_poll_cq(this->rsock->client_id->send_cq, PACKET_WINDOW_SIZE, wc);
    if (ret < 0) {
	perror("ibv_poll_cq");
	exit(1);
    }
    
    // send close msg
    MessageHeader header(MessageType::CLOSE, 0);
    send_buf.write(header);
    this->rsock->post_send(send_buf);

    // check send op is completed
    struct ibv_wc close_wc;
    this->rsock->poll_send_cq(1, &close_wc);
}
