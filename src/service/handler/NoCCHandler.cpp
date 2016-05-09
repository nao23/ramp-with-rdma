#include "NoCCHandler.h"


void NoCCHandler::put_all() {

    Timestamp ts = Timestamp::now();

    // prepare phase
    tbb::parallel_for_each(this->trx->write_set,
    [&](const std::pair<Key, Map<Field, Value>>& w) {
	Key key = w.first;
	Communicator* com = this->cp.get(key);
	com->mtx.lock();
	com->send(MessageType::PREPARE, Item(key, w.second, ts));
	com->recv();
	com->mtx.unlock();
    });

    // commit phase
    send_to_all(MessageType::COMMIT, ts);
}

void NoCCHandler::get_all() {

    ConcurrentMap<Key, Item> ret;
    
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, Set<Field>>& r){
	Key key = r.first;
	Communicator* com = this->cp.get(key);
	com->mtx.lock();
	com->send(MessageType::GET, key);
	com->recv(&ret[key]);
	com->mtx.unlock();
    });
}
