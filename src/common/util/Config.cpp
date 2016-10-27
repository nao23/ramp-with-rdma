#include "Config.h"


Config& Config::get_config() {
    static Config instance;
    return instance;
}

void Config::read_server_list() {
    
    // Get current working directly name
    char cwdname[256];
    memset(cwdname, '\0', 256);
    getcwd(cwdname, 256);
    
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

    char hostname[MAXHOSTNAMELEN];
    char port_str[8];

    while (fscanf(fp, "%s %s\n", hostname, port_str) != EOF)
	this->server_list.push_back(HostAndPort(hostname, port_str));
    
    fclose(fp);    
}

// Create config object (Singletion)
static Config& config = Config::get_config();
