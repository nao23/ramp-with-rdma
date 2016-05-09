#include "ConnectionPool.h"


void ConnectionPool::connect_all() {

    Config& config = Config::get_config();
    if (config.com_type == ComType::TCP || config.com_type == ComType::IPoIB) {
	for (const auto& hp : config.server_list) {
	    this->connections[hp] = TCPSocket::connect(hp);     
	}	
    } else if (config.com_type == ComType::SEND_RECV) {
	for (const auto& hp : config.server_list) {
	    this->connections[hp] = SendRecvSocket::connect(hp);     
	}
    } else if (config.com_type == ComType::RDMA_WRITE) {
	for (const auto& hp : config.server_list) {
	    this->connections[hp] = RDMAWriteSocket::connect(hp);     
	}
    } else if (config.com_type == ComType::RDMA_WRITE_IMM) {
	for (const auto& hp : config.server_list) {
	    this->connections[hp] = RDMAWriteImmSocket::connect(hp);
	}
    }
}

Communicator* ConnectionPool::get(const Key& key) {

    Config& config = Config::get_config();
    size_t hash_v = std::hash<std::string>()(key);
    HostAndPort hp = config.server_list.at(hash_v % config.server_num);
    Communicator* com = this->connections.at(hp);
    this->access_coms.insert(com);

    return com;
}

void ConnectionPool::clear_access_coms() {
    this->access_coms.clear();
}

void ConnectionPool::close_all() {
    
    for (auto& com : this->connections | boost::adaptors::map_values) {
	com->send_close();
	delete com;
    }    
}
