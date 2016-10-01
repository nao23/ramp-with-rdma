#pragma once

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include "ComType.h"
#include "TrxType.h"
#include "HostAndPort.h"
#include "LockDuration.h"


class Config {

private:
    Config() = default;
    Config(const Config &other) = delete;
    Config& operator = (const Config& other) = delete;
    
public:
    int host_id;
    ComType com_type;
    TrxType trx_type;
    int data_num;
    int worker_num;
    int trx_len;
    int write_trx_num;
    int read_trx_num;
    int trx_num;
    int value_size;
    int server_num;
    std::vector<HostAndPort> server_list;
    int max_key_num;
    char* cache_addr;
    size_t cache_size;
    uint32_t cache_rkey;

    LockDuration write_duration;
    LockDuration read_duration;

    bool sleep_flag;
    struct timespec sleep_time;

    static Config& get_config();
    void read_server_list();
    void init();
};
