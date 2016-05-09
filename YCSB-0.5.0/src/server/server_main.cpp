#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "cmdline.h"
#include "Server.h"
#include "TCPServerSocket.h"
#include "SendRecvServerSocket.h"
#include "RDMAWriteServerSocket.h"
#include "RDMAWriteImmServerSocket.h"


int main(int argc, char *argv[]) {

    cmdline::parser parser;
    parser.add<std::string>("trx_type", 't', "transaction type", true, "");
    parser.add<std::string>("com_type", 'c', "communication type", true, "");
    parser.parse_check(argc, argv);

    Config& config = Config::get_config();

    std::string trx_type = parser.get<std::string>("trx_type");
    if (trx_type == "rr") {
	config.trx_type = TrxType::LOCK_BASED;
	config.write_duration = LockDuration::LONG;
	config.read_duration = LockDuration::LONG;
    } else if (trx_type == "rc") {
	config.trx_type = TrxType::LOCK_BASED;
	config.write_duration = LockDuration::LONG;
	config.read_duration = LockDuration::SHORT;
    } else if (trx_type == "ru") {
	config.trx_type = TrxType::LOCK_BASED;
	config.write_duration = LockDuration::LONG;
	config.read_duration = LockDuration::NONE;
    } else if (trx_type == "no_cc") { 
	config.trx_type = TrxType::NO_CONCURRENCY_CONTROL;
    } else if (trx_type == "ramp_f") {
	config.trx_type = TrxType::RAMP_FAST;
    } else if (trx_type == "ac_ramp_f") { 
	config.trx_type = TrxType::AC_RAMP_FAST;
    } else {
	printf("Unkown Trx_Type\n");
	exit(1);
    }

    Acceptor* acc;

    std::string com_type = parser.get<std::string>("com_type");
    char port_str[] = "50000";

    if (com_type == "tcp") {
	config.com_type = ComType::TCP;
	acc = new TCPServerSocket(port_str);
    } else if (com_type == "ipoib") { 
	config.com_type = ComType::IPoIB;
	acc = new TCPServerSocket(port_str);
    } else if (com_type == "send_recv") {
	config.com_type = ComType::SEND_RECV;
	acc = new SendRecvServerSocket(port_str);
    } else if (com_type == "rdma_write") { 
	config.com_type = ComType::RDMA_WRITE;
	acc = new RDMAWriteServerSocket(port_str);
    } else if (com_type == "rdma_write_imm") {
	config.com_type = ComType::RDMA_WRITE_IMM;
	acc = new RDMAWriteImmServerSocket(port_str);	
    } else {
	printf("Unkown Communication Type\n");
	exit(1);
    }
    
    config.sleep_flag = true;
    config.sleep_time = {0, 100};

    Server server(acc);
    server.start();

    return 0;
}
