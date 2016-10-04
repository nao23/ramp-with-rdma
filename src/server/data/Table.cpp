#include "Table.h"


Table::Table() {
    this->logger = spdlog::stdout_logger_mt("Table", true);
}

void Table::prepare(const Item& item) { 
    this->logger->debug("prepare item: {}", item.key);
    this->items[item.ts].push_back(item);
    this->committed_items.invalidate(item.key); 
}

void Table::commit(const Timestamp& ts_c) { 

    this->logger->debug("commit with ts: {}", ts_c.to_str());
    try {
	for (const auto& item : this->items.at(ts_c)) {
	    if (this->latest_commit[item.key] < ts_c) {
		this->latest_commit[item.key] = ts_c;
		this->committed_items.update(item);
	    }
	}
    } catch (std::out_of_range& e) {
	this->logger->error(e.what());
    }
}

void Table::commit(const Key& key, const Timestamp& ts_c) {
    this->logger->debug("commit for key:{} with ts: {}", key, ts_c.to_str());
    if (this->latest_commit[key] < ts_c) {
	this->latest_commit[key] = ts_c;
    }
}

void Table::put(const Item& item) {
    this->logger->debug("put item: {}", item.key);
    prepare(item);
    commit(item.key, item.ts);
}

Item Table::get(const Key& key) {
    this->logger->debug("get item for key: {}", key);
    return get_latest_item(key);
}

Item Table::get_latest_item(const Key& key) { 
    this->logger->debug("get latest item for key: {}" , key);
    return get_item_by_ver(key, this->latest_commit[key]);
}

Item Table::get_item_by_ver(const Key& key, const Timestamp& ts) const {

    this->logger->debug("get item for key: {}, ts: {}", key, ts.to_str());
    try {
	for (const auto& item : this->items.at(ts)) {
	    if (item.key == key) { 
		return item;
	    }
	}
    } catch (std::out_of_range& e) {
	this->logger->error(e.what());
    }    

    return Item();
}
