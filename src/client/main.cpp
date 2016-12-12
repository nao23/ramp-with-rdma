#include "spdlog/spdlog.h"
#include "Client.h"
#include "cmdline.h"


int main(int argc, char *argv[]) {

    // Create a multithreaded color logger
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_logger_mt("main", true);
    // Set global log level to info
    spdlog::set_level(spdlog::level::info); // TODO: can be set dynamically

    // Create a parser, setup and run
    cmdline::parser parser;
    parser.add<std::string>("log_level", 'l', "log level", false, "info");
    parser.add<std::string>("trx_type", 't', "transaction type", true, "");
    parser.add<std::string>("com_type", 'c', "communication type", true, "");
    parser.add<int>("write_trx_num", 'w', "num of write transactions", true, 0);
    parser.add<int>("read_trx_num", 'r', "num of read transactions", true, 0);
    parser.add<int>("data_num", 'n', "num of data", false, 1000);
    parser.add<int>("trx_size", 's', "transaction size (operations)", false, 8);
    parser.add<int>("value_size", 'v', "value size (bytes)", false, 1000);
    parser.parse_check(argc, argv);

    // Set global log level
    std::string log_level = parser.get<std::string>("log_level");
    if (log_level == "info") {
	spdlog::set_level(spdlog::level::info);
    } else if (log_level == "debug") {
	spdlog::set_level(spdlog::level::debug);
    } else {
	spdlog::set_level(spdlog::level::info);
        logger->error("Unkown log level");
        return EXIT_FAILURE;
    }
    
    Config& config = Config::get_config();

    // Get trx type from parser and set it to config object    
    std::string trx_type = parser.get<std::string>("trx_type");
    if (trx_type == "2PL" ) {
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

    // Get com type from parser and set it to config object
    std::string com_type = parser.get<std::string>("com_type");
    if (com_type == "TCP") {
	config.com_type = ComType::TCP;
    } else if (com_type == "IPOIB") { 
	config.com_type = ComType::IPoIB;
    } else if (com_type == "SEND_RECV") {
	config.com_type = ComType::SEND_RECV;
    } else if (com_type == "RDMA_WRITE") { 
	config.com_type = ComType::RDMA_WRITE;
    } else if (com_type == "RDMA_WRITE_IMM") { 
	config.com_type = ComType::RDMA_WRITE_IMM;
    } else {
        logger->error("Unkown com type");
        return EXIT_FAILURE;
    }
    
    config.data_num = parser.get<int>("data_num");
    config.worker_num = 8;
    config.trx_len = parser.get<int>("trx_size");
    config.write_trx_num = parser.get<int>("write_trx_num");
    config.read_trx_num = parser.get<int>("read_trx_num");
    config.value_size = parser.get<int>("value_size");
    config.read_server_list();

    
    Client client;    
    client.start_all();
    client.check_finish();
    client.print_result();

    return EXIT_SUCCESS;
}
