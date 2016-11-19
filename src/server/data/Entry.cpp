#include "Entry.h"


void Entry::invalidate() {
    std::lock_guard<std::mutex> lock(this->mtx);
    Buffer::update(0, IS_INVALID);
}

void Entry::update(const Item& item) {

    std::lock_guard<std::mutex> lock(this->mtx);

    msgpack::pack(this->sbuf, item);
    MessageHeader header(MessageType::RESULT, this->sbuf.size());
    
    size_t offset = sizeof(int);
    Buffer::update(offset, header); 
    offset += sizeof(MessageHeader);
    Buffer::update(offset, this->sbuf.data(), this->sbuf.size());
    offset += this->sbuf.size();
    Buffer::update(offset, IS_ARRIVED);
    Buffer::update(0, IS_VALID);

    this->sbuf.clear();
}

const int Entry::IS_INVALID = 0xffffffff;
const int Entry::IS_ARRIVED = 0xffffffff;
const int Entry::IS_VALID   = 0;
