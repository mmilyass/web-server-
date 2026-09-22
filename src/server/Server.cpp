#include "../../includes/Webserv.hpp"
#include "../response/Response.hpp"
#include "../request/RequestHandler.hpp"

void sighandler(int status)
{
    if (status)
        throwing("");
}

void Server::AddSocketToEpoll(int fd)
{
    epoll_event listener;
    listener.data.fd = fd;
    listener.events = EPOLLIN;
    if (epoll_ctl(_epollInstance, EPOLL_CTL_ADD, fd, &listener) < 0) // we now watching any event in the server.
        throwing("epoll_ctl()");
}

Server::Server(Config &data) : _data(data)
{
    int opt = 1;
    signal(SIGINT, sighandler);
    signal(SIGPIPE, SIG_IGN);
    _data = data;
    CreateEpollInstance();
    for (int i = 0; i < (int)_data.servers[0].listen_ports.size(); i++)
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (fd < 0)
            throwing("socket()");
        addNblock(fd);
        memset((void *)&_address, 0, sizeof(sockaddr_in));
        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(_data.servers[0].listen_ports[i]);
        if (bind(fd, (const struct sockaddr *)&_address, sizeof(struct sockaddr_in)) < 0)
            throwing("bind()");
        if (listen(fd, 1000) < 0)
            throwing("listen()");
        std::cout << "Server running on http://localhost:" << _data.servers[0].listen_ports[i] << std::endl;
        AddSocketToEpoll(fd);

        _ServerFd.push_back(fd);
    }
}

void Server::CreateEpollInstance()
{
    _epollInstance = epoll_create(1); // we create an epoll instance in the kernel
    if (_epollInstance < 0)
        throwing("epoll_create()");
}

int Server::IsSocketFd(int fd)
{
    for (int i = 0; i < (int)_ServerFd.size(); i++)
    {
        if (fd == _ServerFd[i])
            return fd;
    }
    return -1;
}

void Server::run()
{
    while (true)
    {
        readyClients = epoll_wait(_epollInstance, _clients, MAX_CLIENTS_EVENTS, -1);
        if (readyClients < 0)
            throwing("epoll_wait() failed: ");
        for (int i = 0; i < readyClients; i++)
        {
            int fd = _clients[i].data.fd;
            int check = IsSocketFd(fd);
            if (check != -1)
                addClientInEppol(check);
            else if (_ClientsMap.find(fd) != _ClientsMap.end() &&
                     _ClientsMap[fd].CGIfd == fd)
            {
                readCGIPipe(fd);
            }
            else
            {
                if (_clients[i].events & EPOLLIN)
                    readClientRequest(fd);
                if (_clients[i].events & EPOLLOUT)
                    sendHttpResponse(fd);
            }
        }
    }
}

std::map<int, t_clients> Server::getClients() const
{
    return _ClientsMap;
}

std::vector<int> Server::getServerFd()
{
    return _ServerFd;
}

Server::~Server()
{
    _ClientsMap.clear();
    for (int i = 0; i < (int)_ServerFd.size(); i++)
        close(_ServerFd[i]);
}
