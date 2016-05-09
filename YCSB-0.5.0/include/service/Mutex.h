#pragma once

#include <pthread.h>
#include <stdio.h>


class Mutex {

private:
    pthread_rwlock_t mtx;

public:
    Mutex();
    ~Mutex();
    void read_lock();
    void write_lock();
    void lock();
    void unlock();
};
