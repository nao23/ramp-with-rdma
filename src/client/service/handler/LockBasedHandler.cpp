#include "LockBasedHandler.h"


LockBasedHandler::LockBasedHandler(int id) {
    this->logger = spdlog::stdout_logger_mt("LockBasedHandler-" + std::to_string(id), true);
}

void LockBasedHandler::put_all() {

    Timestamp ts = Timestamp::now();

    this->logger->debug("Send PUT requests");
    for (const auto& w : this->trx->write_set) {
	Key key = w.first;
	Communicator* com = this->cp.get(key);
	com->send(MessageType::PUT, Item(key, w.second, ts));
	com->recv();
    }

    this->logger->debug("Send RELEASE_LOCKS requests");
    send_to_all(MessageType::RELEASE_LOCKS);
}

void LockBasedHandler::get_all() {

    std::map<Key, Item> ret;
    
    this->logger->debug("Send GET requests");
    for (const auto& r : this->trx->read_set) {
	Key key = r.first;
	Communicator* com = this->cp.get(key);
	com->send(MessageType::GET, key);
	com->recv(&ret[key]);
    }

    this->logger->debug("Send RELEASE_LOCKS request");
    send_to_all(MessageType::RELEASE_LOCKS);
}
