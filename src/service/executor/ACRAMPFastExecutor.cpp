#include "ACRAMPFastExecutor.h"


ACRAMPFastExecutor::ACRAMPFastExecutor(int id, Communicator* com, Table* table) : RAMPFastExecutor(id, com, table) { 
    this->rdma_com = reinterpret_cast<RDMAWriteSocket*>(this->com);
    this->rdma_com->setup_read_buf();
}

void ACRAMPFastExecutor::run() {

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
	} else if(header.req_type == MessageType::GET_WITH_ADDR) { 
	    Key key;
	    Timestamp ts_req;
	    this->com->recv_body(header.body_size, &key, &ts_req);
            get_item_and_addr(key, ts_req);	
	} else { // close req is received
	    this->com->recv_body(header.body_size);
	    break;
	}
    }
   
    delete this->com;
}

void ACRAMPFastExecutor::get_item_and_addr(Key key, Timestamp ts_req) {

    Item item = this->table->get_latest_item(key);
    RemoteKeyAndAddr rka(this->rdma_com->read_mr->rkey, this->table->committed_items.get_item_addr(key));
    this->com->send(MessageType::RESULT, item, rka);
}
