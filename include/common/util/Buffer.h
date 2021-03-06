#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "spdlog/spdlog.h"


class Buffer {

private:
    static std::shared_ptr<spdlog::logger> class_logger;  // for this class
    char* write_pos;
    char* read_pos;

public:
    char* addr;
    size_t size;    

    Buffer() : write_pos(NULL), read_pos(NULL), addr(NULL) {}
    Buffer(char* addr, size_t size) : write_pos(addr), read_pos(addr), addr(addr), size(size) {}
    static Buffer allocate(size_t size);
    Buffer& update(size_t offset, void* src, size_t len);
    template<class T> Buffer& update(size_t offset, T src) {
	return update(offset, &src, sizeof(T));
    }
    Buffer& append(void* src, size_t len);
    template<class T> Buffer& append(T src) {
	return append(&src, sizeof(T));
    }    
    Buffer& read(void* dst, size_t len);
    template<class T> Buffer& read(T* dst) {
	return read(dst, sizeof(T));
    }
    void free();
    void clear();
};
