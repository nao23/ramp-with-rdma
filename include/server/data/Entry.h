#pragma once

#include <mutex>
#include "Buffer.h"
#include "MessageHeader.h"
#include "Item.h"

#define ENTRY_SIZE (2560)
#define ENTRY_NUM (1000)


class Entry : public Buffer {

private:
    std::mutex mtx;
    msgpack::sbuffer sbuf;

public:
    static const int IS_INVALID;
    static const int IS_ARRIVED;
    static const int IS_VALID;

    Entry() = default;
    Entry(char* addr) : Buffer(addr, ENTRY_SIZE) {}
    void invalidate();
    void update(const Item& item);
};
