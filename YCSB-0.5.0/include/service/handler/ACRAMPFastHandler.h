#pragma once

#include "RAMPFastHandler.h"


class ACRAMPFastHandler : public RAMPFastHandler {

private:
    ConcurrentMap<Key, RemoteKeyAndAddr> addr_cache;

public:
    ACRAMPFastHandler() = default;
    ~ACRAMPFastHandler() = default;

    void get_all();
};
