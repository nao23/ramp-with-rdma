#pragma once

#include <msgpack.hpp>


class RemoteKeyAndAddr {

public:
    uint32_t rkey;
    uint64_t remote_addr;
    MSGPACK_DEFINE(rkey, remote_addr);
    
    //RemoteKeyAndAddr() = default;
    RemoteKeyAndAddr() : rkey(0), remote_addr(0) {}
    RemoteKeyAndAddr(uint32_t rkey, uint64_t remote_addr) {
	this->rkey = rkey;
	this->remote_addr = remote_addr;
    }
    
    RemoteKeyAndAddr(uint32_t rkey, void* remote_addr) {
	this->rkey = rkey;
	this->remote_addr = (uint64_t)(uintptr_t)(remote_addr);
    }
};
