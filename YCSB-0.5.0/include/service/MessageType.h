#pragma once

#include <msgpack.hpp>


enum class MessageType {
    PREPARE,
    COMMIT,
    PUT,
    GET,
    GET_WITH_ADDR,
    DONE,
    RESULT,
    RELEASE_LOCKS,
    CLOSE
};

MSGPACK_ADD_ENUM(MessageType);
