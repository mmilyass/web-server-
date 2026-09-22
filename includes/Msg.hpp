#pragma once
#include "Webserv.hpp"

class Msg {
public:
    // Generic printer with any color
    static void print(const std::string& mode, const std::string& msg, const std::string& color);

    // Fixed log types
    static void info(const std::string& msg);
    static void success(const std::string& msg);
    static void warning(const std::string& msg);
    static void error(const std::string& msg);
    static void debug(const std::string& msg);
};