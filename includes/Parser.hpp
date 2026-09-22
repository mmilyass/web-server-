#pragma once
#include "Webserv.hpp"

class Parser {
public:
    Config parse(const std::vector<std::string> &tokens);
private:
    ServerConfig parseServer(const std::vector<std::string> &tokens, unsigned long &i);
    LocationConfig parseLocation(const std::vector<std::string> &tokens, unsigned long &i);
};