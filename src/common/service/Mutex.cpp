#include "Mutex.h"


Mutex::Mutex() {
    pthread_rwlock_init(&this->mtx, NULL);
}

Mutex::~Mutex() {
    pthread_rwlock_destroy(&this->mtx);
}

void Mutex::read_lock() {
    pthread_rwlock_rdlock(&this->mtx);
}

void Mutex::write_lock() {
    pthread_rwlock_wrlock(&this->mtx);
}

void Mutex::lock() {
    pthread_rwlock_wrlock(&this->mtx);    
}

void Mutex::unlock() {
    pthread_rwlock_unlock(&this->mtx);
}
