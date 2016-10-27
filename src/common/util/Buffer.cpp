#include "Buffer.h"


Buffer Buffer::allocate(size_t size) {

    char* addr;
    if (posix_memalign((void**)&addr, getpagesize(), size) != 0) {
	class_logger->error("posix_memalign: {}", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(addr, 0, size); 
    return Buffer(addr, size);
}

Buffer& Buffer::append(void* src, size_t count) {    
    
    if (this->addr + this->size - this->write_pos < count) { // Segmentation fault
	class_logger->error("Buffer::write(): Buffer has no space for new data");
	exit(EXIT_FAILURE);
    }    
    // Append new data to write_pos and increment write_pos
    memcpy(this->write_pos, src, count);
    this->write_pos += count;
    // Return reference of this object for realizing method chaining 
    return *this;
}

Buffer& Buffer::read(void* dst, size_t count) {

    if (this->addr + this->size - this->read_pos < count) { // Segmentation fault
	class_logger->error("Buffer::read(): You are about to read out of this buffer area");
	exit(EXIT_FAILURE);
    }
    // Read data from read_pos and increment read_pos
    memcpy(dst, this->read_pos, count);
    this->read_pos += count;
    // Return reference of this object for realizing method chaining 
    return *this;
}

void Buffer::free() {
    ::free(this->addr);
}

void Buffer::clear() {
    memset(this->addr, 0, this->size);
    this->write_pos = this->addr;
    this->read_pos = this->addr;
}

std::shared_ptr<spdlog::logger> Buffer::class_logger = spdlog::stdout_logger_mt("Buffer", true);
