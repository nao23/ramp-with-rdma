#include "Item.h"


Item::Item(const Key& key) {
    this->key = key;
}

Item::Item(const Key& key, const std::map<Field, Value>& values, const Timestamp& ts) {
    this->key = key;
    this->values = values;
    this->ts = ts;
}

Item::Item(const Key& key, const std::map<Field, Value>& values, const Timestamp& ts, const std::set<Key>& trx_keys) {
    this->key = key;
    this->values = values;
    this->ts = ts;
    this->trx_keys = trx_keys;
}

bool Item::is_empty() const {
    return (this->key.empty());
}
