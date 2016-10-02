#pragma once

#include <map>
#include <set>
#include "Key.h"
#include "Field.h"
#include "Value.h"
#include "Timestamp.h"


class Transaction {

public:
    std::set<Key> access_keys;
    std::map<Key, std::set<Field>> read_set;
    std::map<Key, std::map<Field, Value>> write_set;
    
    Transaction() = default;
    void add_write(Key key, std::map<Field, Value> values);
    void add_read(Key key, std::set<Field> fields);
    void clear();
};
