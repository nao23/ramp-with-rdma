#include "CommittedItems.h"


CommittedItems::CommittedItems() {

    this->logger = spdlog::stdout_logger_mt("CommittedItemsCache", true);

    if (posix_memalign((void**)&this->addr, getpagesize(), CACHE_SIZE) != 0) {
	perror("posix_memalign");
	exit(1);
    }
    this->size = CACHE_SIZE;
    
    clear();

    for (int i = 0; i < ENTRY_NUM; ++i) {
	this->unused_entries.push(new Entry(this->addr + i * ENTRY_SIZE));
    }
}

void CommittedItems::invalidate(const Key& key) {

    if (this->entries.find(key) == this->entries.end()) {
	this->unused_entries.try_pop(this->entries[key]);
    } else {
	this->entries.at(key)->invalidate();
    }
}

void CommittedItems::update(const Item& item) {    
    try {
	this->entries.at(item.key)->update(item);
    } catch (std::out_of_range& e) {
	this->logger->error(e.what());
    }
}

char* CommittedItems::get_item_addr(const Key& key) {

    if (this->entries.find(key) == this->entries.end()) {
	Entry* entry;
	if (this->unused_entries.try_pop(entry)) {
	    entry->update(Item(key));
	    this->entries[key] = entry;
	}
    }

    return this->entries.at(key)->addr;
}
