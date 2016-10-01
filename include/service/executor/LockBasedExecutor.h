#pragma once

#include "TrxExecutor.h"
#include "LockManager.h"
#include "Config.h"


class LockBasedExecutor : public TrxExecutor {

private:
    LockManager* lock_manager;
    Set<Key> access_keys;
    LockDuration write_duration;
    LockDuration read_duration;

public:
    void put(const Item& item);
    void get(const Key& key);
    void release_locks();

    LockBasedExecutor(int id, Communicator* com, Table* table, LockManager* lock_manager);
    ~LockBasedExecutor() = default;
    void run();
};
