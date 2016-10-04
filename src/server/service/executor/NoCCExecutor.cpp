#include "NoCCExecutor.h"


NoCCExecutor::NoCCExecutor(int id, Communicator* com, Table* table) : TrxExecutor(id, com, table) {
    this->logger = spdlog::stdout_logger_mt("NoCCExecutor-" + std::to_string(id), true);    
}

void NoCCExecutor::run() {
    
    while (true) {

      	MessageHeader header;
	this->com->recv_header(&header);

        if (header.req_type == MessageType::PREPARE) {
            this->logger->debug("Received PREPARE request");
	    Item item;
	    this->com->recv_body(header.body_size, &item);
	    prepare(item);
	} else if (header.req_type == MessageType::COMMIT) {
            this->logger->debug("Received COMMIT request");
	    Timestamp ts_c;
	    this->com->recv_body(header.body_size, &ts_c);
            commit(ts_c);
        } else if (header.req_type == MessageType::GET) {
            this->logger->debug("Received GET request");
	    Key key;
	    this->com->recv_body(header.body_size, &key);
            get(key);
	} else { 
            this->logger->debug("Received CLOSE request");
	    this->com->recv_body(header.body_size);
	    break;
	}
    }
   
    delete this->com;
}

void NoCCExecutor::prepare(const Item& item) {
    this->table->prepare(item);
    this->com->send(MessageType::DONE);
}

void NoCCExecutor::commit(const Timestamp& ts_c) {
    this->table->commit(ts_c);
    this->com->send(MessageType::DONE);
}

void NoCCExecutor::get(const Key& key) {
    this->com->send(MessageType::RESULT, this->table->get_latest_item(key));
}
