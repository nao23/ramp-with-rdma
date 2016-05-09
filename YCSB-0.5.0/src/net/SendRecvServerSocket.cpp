#include "SendRecvServerSocket.h"


Communicator* SendRecvServerSocket::accept() {
    return new SendRecvSocket(this->rsock.accept());
}
