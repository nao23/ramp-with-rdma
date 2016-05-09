#include "TCPServerSocket.h"


TCPServerSocket::TCPServerSocket(char* port_str) {

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    res = NULL;

    if (getaddrinfo(NULL, port_str, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    this->sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (this->sock < 0) {
        perror("socket");
        freeaddrinfo(res);
        exit(1);
    }

    int one = 1;
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one)) < 0) {
	perror("setsockopt");
	freeaddrinfo(res);
	::close(this->sock);
	exit(1);
    }

    one = 1;
    if (setsockopt(this->sock, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(one)) < 0) {
	perror("setsockopt");
	freeaddrinfo(res);
	::close(this->sock);
	exit(1);
    }

    if (::bind(this->sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("bind");
        freeaddrinfo(res);
        ::close(this->sock);
        exit(1);
    }

    freeaddrinfo(res);

    if (listen(this->sock, LISTEN_MAX) < 0) {
        perror("listen");
        ::close(this->sock);
        exit(1);
    }
}

TCPServerSocket::~TCPServerSocket() {
    ::close(this->sock);
}

Communicator* TCPServerSocket::accept() {

    int com = ::accept(this->sock, 0, 0);
    if (com < 0) {
        perror("accept");
        exit(1);
    }
    
    return new TCPSocket(com);
}
