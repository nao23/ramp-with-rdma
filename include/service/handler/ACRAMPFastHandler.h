#pragma once

#include "RAMPFastHandler.h"


class ACRAMPFastHandler : public RAMPFastHandler {

private:
    ConcurrentMap<Key, RemoteKeyAndAddr> addr_cache;

public:
    ACRAMPFastHandler(int id) : RAMPFastHandler(id) {}
    ~ACRAMPFastHandler() = default;

    void get_all();
};
