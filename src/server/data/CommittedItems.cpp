#include "CommittedItems.h"


void CacheEntry::invalidate() {
    
    std::lock_guard<std::mutex> lock(this->mtx);
    
    int is_invalid = 0xffffffff;
    memcpy(this->addr, &is_invalid, sizeof(int));
}

void CacheEntry::update(const Item& item) {

    std::lock_guard<std::mutex> lock(this->mtx);

    msgpack::pack(this->sbuf, item);
    MessageHeader header(MessageType::RESULT, this->sbuf.size());

    int is_invalid = 0;
    int is_arrived = 0xffffffff;

    memcpy(this->addr + sizeof(int), &header, sizeof(MessageHeader));
    memcpy(this->addr + sizeof(int) + sizeof(MessageHeader), this->sbuf.data(), this->sbuf.size());
    memcpy(this->addr + sizeof(int) + sizeof(MessageHeader) + this->sbuf.size(), &is_arrived, sizeof(int));
    
    memcpy(this->addr, &is_invalid, sizeof(int)); // Validate

    this->sbuf.clear();
}


CommittedItems::CommittedItems() {

    this->logger = spdlog::stdout_logger_mt("CommittedItemsCache", true);

    if (posix_memalign((void**)&this->addr, getpagesize(), CACHE_SIZE) != 0) {
	perror("posix_memalign");
	exit(1);
    }
    this->size = CACHE_SIZE;
    
    clear();

    for (int i = 0; i < ENTRY_NUM; ++i) {
	this->unused_entries.push(new CacheEntry(this->addr + i * ENTRY_SIZE));
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
	std::cerr << e.what() << " " << item.key << std::endl;
    }
}

char* CommittedItems::get_item_addr(const Key& key) {

    if (this->entries.find(key) == this->entries.end()) {
	CacheEntry* entry;
	if (this->unused_entries.try_pop(entry)) {
	    entry->update(Item(key));
	    this->entries[key] = entry;
	}
    }

    return this->entries.at(key)->addr;
}
