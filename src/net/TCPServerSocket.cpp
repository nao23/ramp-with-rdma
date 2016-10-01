#include "TCPServerSocket.h"


TCPServerSocket::TCPServerSocket(char* port_str) {
    
    this->logger = spdlog::stdout_logger_mt("TCPServerSocket", true);
    
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    res = NULL;

    if (getaddrinfo(NULL, port_str, &hints, &res) != 0) {
	this->logger->error("getaddrinfo: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    this->sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (this->sock < 0) {
	this->logger->error("socket: {}", strerror(errno));
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    int one = 1;
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one)) < 0) {
	this->logger->error("setsockopt: {}", strerror(errno));
	freeaddrinfo(res);
	close(this->sock);
	exit(EXIT_FAILURE);
    }

    one = 1;
    if (setsockopt(this->sock, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(one)) < 0) {
	this->logger->error("setsockopt: {}", strerror(errno));
	freeaddrinfo(res);
	close(this->sock);
	exit(EXIT_FAILURE);
    }

    if (bind(this->sock, res->ai_addr, res->ai_addrlen) < 0) {
	this->logger->error("bind: {}", strerror(errno));
        freeaddrinfo(res);
        close(this->sock);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    if (listen(this->sock, LISTEN_MAX) < 0) {
	this->logger->error("listen: {}", strerror(errno));
        close(this->sock);
        exit(EXIT_FAILURE);
    }
}

TCPServerSocket::~TCPServerSocket() {
    close(this->sock);
}

Communicator* TCPServerSocket::accept() {

    int com = ::accept(this->sock, 0, 0);
    if (com < 0) {
	this->logger->error("accept: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    return new TCPSocket(com);
}
