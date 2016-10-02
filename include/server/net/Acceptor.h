#pragma once

#include <cerrno>
#include <cstring>
#include "spdlog/spdlog.h"
#include "Communicator.h"

#define LISTEN_MAX (100)


class Acceptor : boost::noncopyable {

protected:
    std::shared_ptr<spdlog::logger> logger;    

public:
    virtual ~Acceptor() = default;
    virtual Communicator* accept() = 0;
};
