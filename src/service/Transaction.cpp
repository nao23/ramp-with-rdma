#include "Transaction.h"


void Transaction::add_write(Key key, std::map<Field, Value> values) {
    this->access_keys.insert(key);
    this->write_set[key] = values;    
}

void Transaction::add_read(Key key, std::set<Field> fields) {
    this->access_keys.insert(key);
    this->read_set[key] = fields;
}

void Transaction::clear() {
    this->access_keys.clear();
    this->read_set.clear();
    this->write_set.clear();
}
