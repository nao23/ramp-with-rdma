#include "RAMPFastHandler.h"


RAMPFastHandler::RAMPFastHandler(int id) {
    this->logger = spdlog::stdout_logger_mt("RAMPFastHandler-" + std::to_string(id), true);
}

void RAMPFastHandler::put_all() {

    Timestamp ts = Timestamp::now();
    
    this->logger->debug("Send PREPARE requests");
    tbb::parallel_for_each(this->trx->write_set,
    [&](const std::pair<Key, std::map<Field, Value>>& w) {
	Key key = w.first;
	this->logger->debug("Key: {}", key);
	Communicator* com = this->cp.get(key);
	com->mtx.lock();
	com->send(MessageType::PREPARE, Item(key, w.second, ts, this->trx->access_keys));
	com->recv();
	com->mtx.unlock();
    });
        
    this->logger->debug("Send COMMIT requests");
    send_to_all(MessageType::COMMIT, ts);
}
			       
void RAMPFastHandler::get_all() {

    ConcurrentMap<Key, Item> ret;

    this->logger->debug("Send GET requests (1st round)");
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, std::set<Field>>& r) {
	Key key = r.first;
	Communicator* com = this->cp.get(key); 
	com->mtx.lock();
	com->send(MessageType::GET, key, Timestamp());
	com->recv(&ret[key]);
	com->mtx.unlock();
    });

    this->logger->debug("Check missing version");
    ConcurrentMap<Key, Timestamp> v_latest;
    for (const auto& item : ret | boost::adaptors::map_values) {
        for (const auto& key : item.trx_keys) {
	    if (v_latest[key] < item.ts) { 
		v_latest[key] = item.ts;
	    }
	}
    }

    this->logger->debug("Send GET requests (2nd round)");
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, std::set<Field>>& r){
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
