#include "Table.h"


void Table::prepare(const Item& item) { 
   
    this->items[item.ts].push_back(item);
    this->committed_items.invalidate(item.key); 
}

void Table::commit(const Timestamp& ts_c) {

    try {
	for (const auto& item : this->items.at(ts_c)) {
	    if (this->latest_commit[item.key] < ts_c) {
		this->latest_commit[item.key] = ts_c;
		this->committed_items.update(item);
	    }
	}
    } catch (std::out_of_range& e) {}
}

void Table::commit(const Key& key, const Timestamp& ts_c) {
    
    if (this->latest_commit[key] < ts_c) {
	this->latest_commit[key] = ts_c;
    }
}

void Table::put(const Item& item) {

    prepare(item);
    commit(item.key, item.ts);
}

Item Table::get(const Key& key) {
    return get_latest_item(key);
}

Item Table::get_latest_item(const Key& key) { 
    return get_item_by_ver(key, this->latest_commit[key]);
}

Item Table::get_item_by_ver(const Key& key, const Timestamp& ts) const {
    
    try {
	for (const auto& item : this->items.at(ts)) {
	    if (item.key == key) { 
		return item;
	    }
	}
    } catch (std::out_of_range& e) {}    

    return Item();
}
