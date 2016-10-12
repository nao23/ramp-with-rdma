#pragma once

#include <unistd.h>
#include <string.h>
#include <tbb/tbb.h>
#include <boost/functional/hash.hpp>


class HostAndPort {

public:
    char hostname[64];
    char port_str[8];
    
    HostAndPort() = default;
    HostAndPort(char* hostname, char* port_str);    
    bool operator == (const HostAndPort& other) const;
};


// for tbb::concurrent_unordered_map
namespace tbb {
    template<>
    struct tbb_hash<HostAndPort> {
	size_t operator()(const HostAndPort& hp) const {
	    size_t seed = 0;
	    boost::hash_combine(seed, hp.hostname);
	    boost::hash_combine(seed, hp.port_str);
	    return seed;
	}
    };
}
