#pragma once

#include <map>
#include <set>
#include "Key.h"
#include "Field.h"
#include "Value.h"
#include "Timestamp.h"


class Item {

public:
    Key key;
    std::map<Field, Value> values;
    Timestamp ts;
    std::set<Key> trx_keys;
    MSGPACK_DEFINE(key, values, ts, trx_keys);

    Item() = default;
    Item(const Key& key) : key(key) {}
    Item(const Key& key, const std::map<Field, Value>& values, const Timestamp& ts) 
	: key(key), values(values), ts(ts) {}
    Item(const Key& key, const std::map<Field, Value>& values, const Timestamp& ts, const std::set<Key>& trx_keys) 
	: key(key), values(values), ts(ts), trx_keys(trx_keys) {}
    bool is_empty() const;
};
