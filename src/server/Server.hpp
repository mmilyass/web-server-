#pragma once

#include "../../includes/Webserv.hpp"

class Response;

#define MAX_CLIENTS_EVENTS 1000

class Config;

typedef struct s_clients
{
    s_clients() : fd(-1), firstTime(true), CGIfd(-1), leftData(false), bytesread(0), clsResponse(NULL), last_activity(0), cgi_headers_parsed(false), pid(-1) {}
    int fd;
    std::string request;
    std::string response;
    bool firstTime;
    int CGIfd;
    bool leftData;
    ssize_t bytesread;
    std::string DataLeft;
    Response *clsResponse;
    time_t last_activity; // ADD THIS LINE
    epoll_event event;
    bool cgi_headers_parsed;
    int pid;
} t_clients;

class Server
{
private:
    int _epollInstance;
    std::vector<int> _ServerFd;
    Config &_data;
    struct sockaddr_in _address;
    std::map<int, t_clients> _ClientsMap;
    epoll_event _clients[MAX_CLIENTS_EVENTS];

public:
    int readyClients;
    Server(Config &data);
    std::vector<int> getServerFd();
    void addClientInEppol(int fd);
    void readClientRequest(unsigned int clientFd);
    void readCGIPipe(unsigned int pipeFd);
    void AddSocketToEpoll(int fd);
    void sendHttpResponse(int clientFd);
    void CreateEpollInstance();
    void deleteClientFromEpoll(unsigned int clientFd);
    void addNblock(unsigned int clientFd);
    void getReadInfos(unsigned int fd);
    void errorSending(unsigned int fd);
    void ReadSend(unsigned int fd);
    int IsSocketFd(int fd);
    void ifCGI(int fd);
    void modifySockEvents(int epollfd, int fd);
    // void Server::isCGIPipe(int fd);
    void readCGIPipe(int pipeFd);
    void SendErrorPage(int fd, std::string code);
    size_t contentLenght(std::string header);
    void run();
    std::map<int, t_clients> getClients() const;
    ~Server();
};

void throwing(std::string fct);
