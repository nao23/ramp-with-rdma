#pragma once

#include <map>
#include <array>
#include <vector>
#include <set>
#include <tbb/tbb.h>

// Sequential Containers
template<class KEY, class VAL> using Map = std::map<KEY, VAL>;
template<class VAL, std::size_t SIZE> using Array = std::array<VAL, SIZE>;
template<class VAL> using Vector = std::vector<VAL>;
template<class VAL> using Set = std::set<VAL>;

// Concurrent Containers
template<class KEY, class VAL> using ConcurrentMap = tbb::concurrent_unordered_map<KEY, VAL>;
template<class VAL> using ConcurrentVector = tbb::concurrent_vector<VAL>;
template<class VAL> using ConcurrentQueue = tbb::concurrent_queue<VAL>;
template<class VAL> using ConcurrentSet = tbb::concurrent_unordered_set<VAL>;
