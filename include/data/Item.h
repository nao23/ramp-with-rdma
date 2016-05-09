#pragma once

#include "Containers.h"
#include "Key.h"
#include "Field.h"
#include "Value.h"
#include "Timestamp.h"


class Item {

public:
    Key key;
    Map<Field, Value> values;
    Timestamp ts;
    Set<Key> trx_keys;
    MSGPACK_DEFINE(key, values, ts, trx_keys);

    Item() = default;
    Item(const Key& key);
    Item(const Key& key, const Map<Field, Value>& values, const Timestamp& ts);
    Item(const Key& key, const Map<Field, Value>& values, const Timestamp& ts, const Set<Key>& trx_keys);
    bool is_empty() const;
};
