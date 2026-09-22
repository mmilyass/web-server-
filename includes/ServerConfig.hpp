#pragma once
#include "LocationConfig.hpp"
#include "Webserv.hpp"
class ServerConfig {
public:
    std::map<std::string, std::map<std::string, std::string> > sessions;
    //int listen_port;
    std::vector<int> listen_ports;
    std::string index;
    std::string server_name;
    std::string root;
    size_t client_max_body_size;
    std::map<int, std::string> error_pages;
    std::vector<LocationConfig> locations;
};