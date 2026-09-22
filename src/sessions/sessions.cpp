#include "../../includes/Webserv.hpp"

std::string generateSessionId()
{
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string s;
    for (int i = 0; i < 16; ++i)
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    return s;
}

void createNewSession(Request &req, ServerConfig &config)
{
    std::string id = generateSessionId();

    config.sessions[id]["username"] = "Soufiane";

    req.setHeader("Set-Cookie", "session_id=" + id + "; HttpOnly; Path=/; Max-Age=30");
}

std::string getCookieValue(const Request &req, std::string key)
{
    std::map<std::string, std::string>::const_iterator it = req.cookies.find(key);
    if (it != req.cookies.end())
        return it->second;
    return "";
}


