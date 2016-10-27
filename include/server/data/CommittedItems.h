#pragma once

#include <iostream>
#include <malloc.h>
#include <mutex>
#include "Buffer.h"
#include "Containers.h"
#include "MessageHeader.h"
#include "Item.h"


#define ENTRY_SIZE (2560)
#define ENTRY_NUM (1000)
#define CACHE_SIZE (ENTRY_NUM * ENTRY_SIZE)


class CacheEntry : public Buffer {

private:
    std::mutex mtx;
    msgpack::sbuffer sbuf;

public:
    CacheEntry() = default;
    CacheEntry(char* addr) : Buffer(addr, ENTRY_SIZE) {}

    void invalidate();
    void update(const Item& item);
};


class CommittedItems : public Buffer {

private:
    std::shared_ptr<spdlog::logger> logger;
    ConcurrentMap<Key, CacheEntry*> entries;
    ConcurrentQueue<CacheEntry*> unused_entries;

public:
    CommittedItems();
  
    void invalidate(const Key& key);
    void update(const Item& item);
    char* get_item_addr(const Key& key);
};
