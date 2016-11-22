#pragma once

#include "TrxExecutor.h"
#include "LockManager.h"
#include "Config.h"


class LockBasedExecutor : public TrxExecutor {

private:
    LockManager* lock_manager;
    std::set<Key> access_keys;

public:
    void put(const Item& item);
    void get(const Key& key);
    void release_locks();

    LockBasedExecutor(int id, Communicator* com, Table* table, LockManager* lock_manager);
    ~LockBasedExecutor() = default;
    void run();
};
