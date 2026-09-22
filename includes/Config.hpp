#pragma once

class Config {
public:
    std::map<std::string, std::string>  MimeTypes;
    std::vector<ServerConfig> servers;
};