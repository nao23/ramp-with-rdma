#include "LockBasedHandler.h"


void LockBasedHandler::put_all() {

    Timestamp ts = Timestamp::now();

    for (const auto& w : this->trx->write_set) {
	Key key = w.first;
	Communicator* com = this->cp.get(key);
	com->send(MessageType::PUT, Item(key, w.second, ts));
	com->recv();
    }

    send_to_all(MessageType::RELEASE_LOCKS);
}

void LockBasedHandler::get_all() {

    Map<Key, Item> ret;

    for (const auto& r : this->trx->read_set) {
	Key key = r.first;
	Communicator* com = this->cp.get(key);
	com->send(MessageType::GET, key);
	com->recv(&ret[key]);
    }

    send_to_all(MessageType::RELEASE_LOCKS);
}
