#pragma once

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include "ComType.h"
#include "TrxType.h"
#include "HostAndPort.h"


class Config {

private:
    Config() = default;
    Config(const Config &other) = delete;
    Config& operator = (const Config& other) = delete;
    
public:
    ComType com_type;
    TrxType trx_type;
    int data_num;
    int worker_num;
    int trx_len;
    int write_trx_num;
    int read_trx_num;
    int value_size;
    std::vector<HostAndPort> server_list;
    int max_key_num;
    char* cache_addr;
    size_t cache_size;
    uint32_t cache_rkey;

    static Config& get_config();
    void read_server_list();
};
