#pragma once

#include "Containers.h"
#include "Item.h"
#include "CommittedItemsCache.h"


class Table {

public:
    ConcurrentMap<Timestamp, ConcurrentVector<Item>> items;
    ConcurrentMap<Key, Timestamp> latest_commit;
    CommittedItemsCache committed_items;

    Table() = default;
    void prepare(const Item& item);
    void commit(const Timestamp& ts_c);
    void commit(const Key& key, const Timestamp& ts_c);
    void put(const Item& item);
    Item get(const Key& key);
    Item get_latest_item(const Key& key);
    Item get_item_by_ver(const Key& key, const Timestamp& ts_req) const;
};
