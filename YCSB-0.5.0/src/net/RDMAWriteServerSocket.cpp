#include "RDMAWriteServerSocket.h"


Communicator* RDMAWriteServerSocket::accept() {
    return new RDMAWriteSocket(this->rsock.accept());
}

