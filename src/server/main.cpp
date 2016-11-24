#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "cmdline.h"
#include "spdlog/spdlog.h"
#include "Server.h"
#include "TCPServerSocket.h"
#include "SendRecvServerSocket.h"
#include "RDMAWriteServerSocket.h"
#include "RDMAWriteImmServerSocket.h"


int main(int argc, char *argv[]) {
    
    // Create a multithreaded color logger
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_logger_mt("main", true);
    // Set global log level to info
    spdlog::set_level(spdlog::level::info); // TODO: can be set dynamically
    
    // Create a parser, setup and run
    cmdline::parser parser;
    parser.add<std::string>("trx_type", 't', "transaction type", true, "");
    parser.add<std::string>("com_type", 'c', "communication type", true, "");
    parser.parse_check(argc, argv);
    
    Config& config = Config::get_config();

    // Get trx type from parser and set it to config object
    std::string trx_type = parser.get<std::string>("trx_type");
    if (trx_type == "2PL") {
	config.trx_type = TrxType::TWO_PHASE_LOCKING;
    } else if (trx_type == "NO_CC") { 
	config.trx_type = TrxType::NO_CONCURRENCY_CONTROL;
    } else if (trx_type == "RAMP_F") {
	config.trx_type = TrxType::RAMP_FAST;
    } else if (trx_type == "AC_RAMP_F") { 
	config.trx_type = TrxType::AC_RAMP_FAST;
    } else {
	logger->error("Unkown trx type");
	return EXIT_FAILURE;
    }
    
    Acceptor* acc;
    char port_str[] = "50000";
    
    // Get com type from parser and set it to config object
    std::string com_type = parser.get<std::string>("com_type");
    if (com_type == "TCP") {
	config.com_type = ComType::TCP;
	acc = new TCPServerSocket(port_str);
    } else if (com_type == "IPOIB") { 
	config.com_type = ComType::IPoIB;
	acc = new TCPServerSocket(port_str);
    } else if (com_type == "SEND_RECV") {
	config.com_type = ComType::SEND_RECV;
	acc = new SendRecvServerSocket(port_str);
    } else if (com_type == "RDMA_WRITE") { 
	config.com_type = ComType::RDMA_WRITE;
	acc = new RDMAWriteServerSocket(port_str);
    } else if (com_type == "RDMA_WRITE_IMM") {
	config.com_type = ComType::RDMA_WRITE_IMM;
	acc = new RDMAWriteImmServerSocket(port_str);	
    } else { 
	logger->error("Unkown communication type");
	return EXIT_FAILURE;
    }

    
    Server server(acc);
    server.start();

    return EXIT_SUCCESS;
}
