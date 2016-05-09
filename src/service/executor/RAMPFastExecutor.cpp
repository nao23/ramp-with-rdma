#include "RAMPFastExecutor.h"


void RAMPFastExecutor::run() {
    
    while (true) {

      	MessageHeader header;
	this->com->recv_header(&header);

        if (header.req_type == MessageType::PREPARE) {
	    Item item;
	    this->com->recv_body(header.body_size, &item);
	    prepare(item);
	} else if (header.req_type == MessageType::COMMIT) {
	    Timestamp ts_c;
	    this->com->recv_body(header.body_size, &ts_c);
            commit(ts_c);
        } else if (header.req_type == MessageType::GET) {
	    Key key;
	    Timestamp ts_req;
	    this->com->recv_body(header.body_size, &key, &ts_req);
            get(key, ts_req);
	} else { // close req is received
	    this->com->recv_body(header.body_size);
	    break;
	}
    }
   
    delete this->com;
}

void RAMPFastExecutor::prepare(const Item& item) {

    this->table->prepare(item);
    this->com->send(MessageType::DONE);
}

void RAMPFastExecutor::commit(const Timestamp& ts_c) {

    this->table->commit(ts_c);
    this->com->send(MessageType::DONE);
}

void RAMPFastExecutor::get(const Key& key, const Timestamp& ts_req) {

    if (ts_req.is_empty()) {
	this->com->send(MessageType::RESULT, this->table->get_latest_item(key));
    } else {
	this->com->send(MessageType::RESULT, this->table->get_item_by_ver(key, ts_req));
    }
}
