#pragma once
#include "Webserv.hpp"


class LocationConfig {
public:
    std::string path;
    std::string root;
    std::string index;
    bool autoindex;
    std::string list;
    std::vector<std::string> methods;
    std::string upload_path;
    std::string cgi_path;
    // (bonus)
    int return_code;
    std::string return_url;
};