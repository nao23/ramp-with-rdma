#include "ACRAMPFastHandler.h"


void ACRAMPFastHandler::get_all() {

    ConcurrentMap<Key, Item> ret;
    
    // 1st round
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, Set<Field>>& r) {
	Key key = r.first;
	RDMAWriteSocket* com = reinterpret_cast<RDMAWriteSocket*>(this->cp.get(key));
	com->mtx.lock();
	if (this->addr_cache.find(key) == this->addr_cache.end()) {
	    com->send(MessageType::GET_WITH_ADDR, key, Timestamp());
	    com->recv(&ret[key], &this->addr_cache[key]);
	} else {
	    if (com->read(&ret[key], this->addr_cache[key]) == -1) {
		com->send(MessageType::GET_WITH_ADDR, key, Timestamp());
		com->recv(&ret[key], &this->addr_cache[key]);
	    }
	}
	com->mtx.unlock();
    });
    
    // check missing version
    ConcurrentMap<Key, Timestamp> v_latest;
    for (const auto& item : ret | boost::adaptors::map_values) {
        for (const auto& key : item.trx_keys) {
            if (v_latest[key] < item.ts) {
                v_latest[key] = item.ts;
            }
        }
    }
    
    // 2nd round
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, Set<Field>>& r) {
	Key key = r.first;
	if (v_latest[key] > ret[key].ts) {
	    Communicator* com = this->cp.get(key);
	    com->mtx.lock();
	    com->send(MessageType::GET, key, v_latest[key]);
	    com->recv(&ret[key]);
	    com->mtx.unlock();
	}
    });
}
