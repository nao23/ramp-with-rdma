#include "Config.h"


Config& Config::get_config() {
    static Config instance;
    return instance;
}

void Config::read_server_list() {
    
    // get current working directly name
    char cwdname[256];
    memset(cwdname, '\0', 256);
    getcwd(cwdname, 256);
    
    //std::string filename("/home/murata/ramp-with-rdma/server_lists/");
    std::string filename(cwdname);
    
    filename += "/server_lists";

    if (this->com_type == ComType::TCP) {
	filename += "/tcp";
    } else {
	filename += "/ib";
    }
    
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char hostname[64];
    char port_str[8];

    while (fscanf(fp, "%s %s\n", hostname, port_str) != EOF)
	this->server_list.push_back(HostAndPort(hostname, port_str));
    
    fclose(fp);
    
    this->server_num = this->server_list.size();
}

void Config::init() {
    
    char hostname[64];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
	perror("gethostname");
	exit(1);
    }

    char host_id_str[3];
    host_id_str[0] = hostname[5];
    host_id_str[1] = hostname[6];
    host_id_str[2] = '\0';

    this->host_id = atoi(host_id_str);
}

static Config& config = Config::get_config();
