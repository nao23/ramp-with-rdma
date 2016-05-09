#pragma once

#include "Containers.h"
#include "Key.h"
#include "Field.h"
#include "Value.h"
#include "Timestamp.h"


class Transaction {

public:
    Set<Key> access_keys;
    Map<Key, Set<Field>> read_set;
    Map<Key, Map<Field, Value>> write_set;
    
    Transaction() = default;
    void add_write(Key key, Map<Field, Value> values);
    void add_read(Key key, Set<Field> fields);
    void clear();
};
