#include "LockManager.h"


void LockManager::read_lock(const Key& key) {
    this->mutex_table[key].read_lock();
}

void LockManager::write_lock(const Key& key) {
    this->mutex_table[key].write_lock();
}

void LockManager::unlock(const Key& key) {
    this->mutex_table.at(key).unlock();
}

void LockManager::unlock(const Set<Key>& keys) {
    
    for (const auto& key : keys) {
	this->mutex_table.at(key).unlock();
    }
}
