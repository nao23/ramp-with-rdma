#pragma once

#include "Communicator.h"

#define LISTEN_MAX (100)


class Acceptor : boost::noncopyable {

public:
    virtual ~Acceptor() = default;
    virtual Communicator* accept() = 0;
};
