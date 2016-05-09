#include "RAMPFastHandler.h"


void RAMPFastHandler::put_all() {

    Timestamp ts = Timestamp::now();
    
    // prepare phase
    tbb::parallel_for_each(this->trx->write_set,
    [&](const std::pair<Key, Map<Field, Value>>& w) {
	Key key = w.first;
	Communicator* com = this->cp.get(key);
	com->mtx.lock();
	com->send(MessageType::PREPARE, Item(key, w.second, ts, this->trx->access_keys));
	com->recv();
	com->mtx.unlock();
    });
        
    // commit phase
    send_to_all(MessageType::COMMIT, ts);
}
			       
void RAMPFastHandler::get_all() {

    ConcurrentMap<Key, Item> ret;

    // 1st round
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, Set<Field>>& r) {
	Key key = r.first;
	Communicator* com = this->cp.get(key); 
	com->mtx.lock();
	com->send(MessageType::GET, key, Timestamp());
	com->recv(&ret[key]);
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
    [&](const std::pair<Key, Set<Field>>& r){
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
