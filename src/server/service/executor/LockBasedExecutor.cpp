#include "LockBasedExecutor.h"


LockBasedExecutor::LockBasedExecutor(int id, Communicator* com, Table* table, LockManager* lock_manager) : TrxExecutor(id, com, table) {
    this->logger = spdlog::stdout_logger_mt("LockBasedExecutor-" + std::to_string(id), true);
    this->lock_manager = lock_manager;
    Config& config = Config::get_config();
    this->write_duration = config.write_duration;
    this->read_duration = config.read_duration;
}

void LockBasedExecutor::run() {

    while (true) {

	MessageHeader header;
	this->com->recv_header(&header);
		
	if (header.req_type == MessageType::PUT) {
            this->logger->debug("Received PUT request");
	    Item item;
	    this->com->recv_body(header.body_size, &item);
	    put(item);
	} else if (header.req_type == MessageType::GET) {
            this->logger->debug("Received GET request");
	    Key key;
	    this->com->recv_body(header.body_size, &key);
	    get(key);
	} else if (header.req_type == MessageType::RELEASE_LOCKS) {
            this->logger->debug("Received RELEASE_LOCKS request");
	    this->com->recv_body(header.body_size);
	    release_locks();
	} else {
            this->logger->debug("Received CLOSE request");
	    this->com->recv_body(header.body_size);
	    break;
	}
    }
    
    delete this->com;
}

void LockBasedExecutor::put(const Item& item) {

    this->lock_manager->write_lock(item.key);
    this->table->put(item);
    
    if (this->write_duration == LockDuration::SHORT) {
	this->lock_manager->unlock(item.key);
    } else if (this->write_duration == LockDuration::LONG) {
	this->access_keys.insert(item.key);
    }

    this->com->send(MessageType::DONE);
}

void LockBasedExecutor::get(const Key& key) {

    if (this->read_duration != LockDuration::NONE) {
	this->lock_manager->read_lock(key);
    }
    
    Item item = this->table->get(key);

    if (this->read_duration == LockDuration::SHORT) {
	this->lock_manager->unlock(key);
    } else if (this->read_duration == LockDuration::LONG) {
	this->access_keys.insert(key);
    }

    this->com->send(MessageType::RESULT, item);
}

void LockBasedExecutor::release_locks() {
    this->lock_manager->unlock(this->access_keys);
    this->access_keys.clear();
    this->com->send(MessageType::DONE);
}
