#pragma once
#include "../../includes/Webserv.hpp"

class cgi
{

public:
    int _pipeFd;
    int _Pid;
    void CGIhandler(stCgiInfo &info);
    int runCGI(stCgiInfo &info);
    std::string getRunnerFor(const std::string &filename);

    cgi() : _pipeFd(-1) {}
    ~cgi() {}
};
