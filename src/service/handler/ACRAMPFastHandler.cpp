#include "ACRAMPFastHandler.h"


void ACRAMPFastHandler::get_all() {

    ConcurrentMap<Key, Item> ret;

    this->logger->debug("Send GET_WITH_ADDR request"); 
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
    

    ConcurrentMap<Key, Timestamp> v_latest;

    this->logger->debug("Check missing version");
    for (const auto& item : ret | boost::adaptors::map_values) {
        for (const auto& key : item.trx_keys) {
            if (v_latest[key] < item.ts) {
                v_latest[key] = item.ts;
            }
        }
    }    

    
    this->logger->debug("Send GET requests");    
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
