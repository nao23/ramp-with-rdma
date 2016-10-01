#pragma once

#include <tbb/tbb.h>

// Concurrent Containers
template<class KEY, class VAL> using ConcurrentMap = tbb::concurrent_unordered_map<KEY, VAL>;
template<class VAL> using ConcurrentVector = tbb::concurrent_vector<VAL>;
template<class VAL> using ConcurrentQueue = tbb::concurrent_queue<VAL>;
template<class VAL> using ConcurrentSet = tbb::concurrent_unordered_set<VAL>;
