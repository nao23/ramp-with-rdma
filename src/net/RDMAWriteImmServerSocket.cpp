#include "RDMAWriteImmServerSocket.h"


Communicator* RDMAWriteImmServerSocket::accept() {
    return new RDMAWriteImmSocket(this->rsock.accept());
}

