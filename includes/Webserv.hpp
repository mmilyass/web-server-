#pragma once

#include <iostream>
// # include <fstream>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
// # include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <cstdarg>

/* STL Containers */
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>

/* System */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
// # include <machine/types.h>
#include <signal.h>

/* Network */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>

// epoll
#include <sys/epoll.h>

// dir
#include <dirent.h>

// colors
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define LIGHT_RED "\x1B[91m"
#define WHITE "\x1B[37m"
#define BLINK "\x1b[5m"`
#define YELLOW "\x1B[33m"
#define LIGHT_BLUE "\x1B[94m"
#define CYAN "\x1B[36m"
#define DARK_GREY "\x1B[90m"
#define LIGHTMAGENTA "\x1B[95m"
#define GREEN "\x1B[32m"
#define LIGHT_GREEN "\x1B[92m"
#include "ServerConfig.hpp"
#include "Config.hpp"
#include "Msg.hpp"
#include "Parser.hpp"
#include "../src/request/Request.hpp"
#include "../src/server/Server.hpp"

class ConfigFileReader
{
public:
	static std::string read(const std::string &path);
};

class Tokenizer
{
public:
	std::vector<std::string> tokenize(const std::string &content);
};

void parse_config(Config &config);

template <typename T>
std::string to_string(T &value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

struct stCgiInfo
{
	stCgiInfo() : FileName(""), QueryString(""), PathInfo(""),
				  Method(""), ContentLenght(-1), Body(""), ContentType(""), HttpV(""),
				  ServerName(""), Port(0) {}
	stCgiInfo(const stCgiInfo &Other) : FileName(Other.FileName), QueryString(Other.QueryString),
										PathInfo(Other.PathInfo), Method(Other.Method), ContentLenght(Other.ContentLenght),
										Body(Other.Body), ContentType(Other.ContentType), HttpV(Other.HttpV), ServerName(Other.ServerName), Port(Other.Port) {}
	std::string FileName;
	std::string QueryString;
	std::string PathInfo;
	std::string Method;
	ssize_t ContentLenght;
	std::string Body;
	std::string	ContentType;
	std::string	HttpV;
	std::string	ServerName;
	int			Port;
};

#include "../src/CGI/cgi.hpp"

class Request;
class RequestHandler;

std::string getCookieValue(const Request &req, std::string key);
void createNewSession(Request &req, ServerConfig &config);
std::string generateSessionId();
std::string buildDirectoryListing(const std::string &directoryPath);