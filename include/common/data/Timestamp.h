#pragma once

#include <sys/time.h>
#include <string>
#include <msgpack.hpp>
#include <tbb/tbb.h>
#include <boost/functional/hash.hpp>


class Timestamp {

public:
    long sec, usec;
    MSGPACK_DEFINE(sec, usec);

    Timestamp() : sec(0), usec(0) {}
    Timestamp(struct timeval& time) : sec(time.tv_sec), usec(time.tv_usec) {}
    bool operator == (const Timestamp& other) const;
    bool operator > (const Timestamp& other) const;
    bool operator < (const Timestamp& other) const;
    bool is_empty() const;
    std::string to_str() const;
    static Timestamp now();
    static double get_elapsed_sec(const Timestamp& bgn, const Timestamp& end);
};


// for tbb::concurrent_unordered_map
namespace tbb {
    template<>
    struct tbb_hash<Timestamp> {
	tbb_hash() {}
	size_t operator()(const Timestamp& ts) const {

	    size_t seed = 0;
	    boost::hash_combine(seed, ts.sec);
	    boost::hash_combine(seed, ts.usec);

	    return seed;
	}
    };
}
