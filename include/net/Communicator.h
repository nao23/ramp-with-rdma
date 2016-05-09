#pragma once

#include <boost/noncopyable.hpp>
#include <msgpack.hpp>
#include "Mutex.h"
#include "MessageHeader.h"

#define PACKET_SIZE (2560)
#define PACKET_WINDOW_SIZE (10)


class Communicator : boost::noncopyable {

private:
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk;
    msgpack::unpacker unpk;
    
    void pack_args() {}
    template<class First, class... Rest> void pack_args(First first, Rest... rest) {
	this->pk.pack(first);
	pack_args(rest...);
    }

    void unpack_args() {}
    template<class First, class... Rest> void unpack_args(First first, Rest... rest) {
	msgpack::unpacked res;
	this->unpk.next(&res);
	msgpack::object obj = res.get();
	obj.convert(first);
	unpack_args(rest...);
    }
    
public:
    Mutex mtx;

    Communicator() : pk(&sbuf) {}
    virtual ~Communicator() = default;
    
    template<class... Args> void send(MessageType msg_type, Args... args) {
      	pack_args(args...); 
     	send_msg(MessageHeader(msg_type, this->sbuf.size()), this->sbuf.data());
	this->sbuf.clear();
    }
    
    template<class... Args> void recv(Args... args) {
	MessageHeader header;
	recv_header(&header);
	recv_body(header.body_size, args...);
    }

    template<class... Args> void recv_body(size_t body_size, Args... args) {
        this->unpk.reserve_buffer(body_size);
       	memcpy(this->unpk.buffer(), get_body(body_size), body_size);
	this->unpk.buffer_consumed(body_size);
	unpack_args(args...);
	clear_msg_buf();
    }

    virtual void send_msg(MessageHeader header, char* body) = 0;    
    virtual void recv_header(MessageHeader* header) = 0;
    virtual char* get_body(size_t body_size) = 0;
    virtual void clear_msg_buf() = 0;
    virtual void send_close() = 0;
};
