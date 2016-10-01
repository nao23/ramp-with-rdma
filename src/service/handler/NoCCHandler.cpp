#include "NoCCHandler.h"


NoCCHandler::NoCCHandler(int id) {
    this->logger = spdlog::stdout_logger_mt("NoCCHandler-" + std::to_string(id), true);
}

void NoCCHandler::put_all() {

    Timestamp ts = Timestamp::now();

    this->logger->debug("Send PREPARE requests");
    tbb::parallel_for_each(this->trx->write_set,
    [&](const std::pair<Key, std::map<Field, Value>>& w) {
	Key key = w.first;
	Communicator* com = this->cp.get(key);
	com->mtx.lock();
	com->send(MessageType::PREPARE, Item(key, w.second, ts));
	com->recv();
	com->mtx.unlock();
    });

    this->logger->debug("Commit phase");
    send_to_all(MessageType::COMMIT, ts);
}

void NoCCHandler::get_all() {

    ConcurrentMap<Key, Item> ret;  

    this->logger->debug("Send GET requests");
    tbb::parallel_for_each(this->trx->read_set,
    [&](const std::pair<Key, std::set<Field>>& r){
	Key key = r.first;
	Communicator* com = this->cp.get(key);
	com->mtx.lock();
	com->send(MessageType::GET, key);
	com->recv(&ret[key]);
	com->mtx.unlock();
    });
}
