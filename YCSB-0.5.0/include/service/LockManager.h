#pragma once

#include <boost/noncopyable.hpp>
#include "Containers.h"
#include "Key.h"
#include "Mutex.h"


class LockManager : boost::noncopyable {

private:
    ConcurrentMap<Key, Mutex> mutex_table;

public:
    void read_lock(const Key& key);
    void write_lock(const Key& key);
    void unlock(const Key& key);
    void unlock(const Set<Key>& keys);
};
