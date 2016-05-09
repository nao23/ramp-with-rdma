#pragma once

#include <msgpack.hpp>


enum class LockDuration {
    LONG,
    SHORT,
    NONE
};

MSGPACK_ADD_ENUM(LockDuration);
