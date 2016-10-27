#pragma once

#include "spdlog/spdlog.h"
#include "Containers.h"
#include "Item.h"
#include "CommittedItems.h"


class Table {

private:
    std::shared_ptr<spdlog::logger> logger;    

public:
    ConcurrentMap<Timestamp, ConcurrentVector<Item>> items;
    ConcurrentMap<Key, Timestamp> latest_commit;
    CommittedItems committed_items;

    Table();
    void prepare(const Item& item);
    void commit(const Timestamp& ts_c);
    void commit(const Key& key, const Timestamp& ts_c);
    void put(const Item& item);
    inline Item get(const Key& key) {
	return get_latest_item(key);
    }
    inline Item get_latest_item(const Key& key) {
	return get_item_by_ver(key, this->latest_commit[key]);	
    }
    Item get_item_by_ver(const Key& key, const Timestamp& ts_req) const;
};
