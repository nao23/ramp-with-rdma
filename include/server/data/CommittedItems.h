#pragma once

#include <iostream>
#include <malloc.h>
#include "Containers.h"
#include "Entry.h"

#define CACHE_SIZE (ENTRY_NUM * ENTRY_SIZE)


class CommittedItems : public Buffer {

private:
    std::shared_ptr<spdlog::logger> logger;
    ConcurrentMap<Key, Entry*> entries;
    ConcurrentQueue<Entry*> unused_entries;

public:
    CommittedItems();  
    void invalidate(const Key& key);
    void update(const Item& item);
    char* get_item_addr(const Key& key);
};
