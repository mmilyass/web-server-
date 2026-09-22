#include "../../includes/Webserv.hpp"
#include "../response/Response.hpp"
#include "../request/RequestHandler.hpp"

void Server::SendErrorPage(int sockfd, std::string code)
{
    std::ostringstream errorPath;
    std::ostringstream oss;
    std::string header;
    errorPath << "errors/" << code << ".html";
    int fd = open(errorPath.str().c_str(), O_RDONLY);
    char buffer[2048];
    int bytes = read(fd, buffer, sizeof(buffer));
    buffer[bytes] = '\0';
    std::string body = buffer;
    oss << body.size();
    header =
        "HTTP/1.1 " + code + " OK\r\nContent-Type: text/html\r\nContent-Length: " + oss.str() + "\r\nConnection: close\r\n\r\n";
    header += body;
    send(sockfd, header.c_str(), header.length(), 0);
}

void Server::addNblock(unsigned int fd)
{
    if ((int)fd < 0)
        return;

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        throwing("fcntl(F_GETFL)");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        throwing("fcntl(F_SETFL)");
}

void Server::readCGIPipe(int pipeFd)
{
    int sock = _ClientsMap[pipeFd].fd;
    char buffer[1024];

    if (std::time(NULL) - _ClientsMap[pipeFd].last_activity > 2)
    {
        kill(_ClientsMap[pipeFd].pid, SIGKILL);
        SendErrorPage(sock, "408");
        deleteClientFromEpoll(pipeFd);
        deleteClientFromEpoll(sock);
        return;
    }

    ssize_t b_read = read(pipeFd, buffer, sizeof(buffer));
    if (b_read < 0)
    {
        SendErrorPage(sock, "500");
        deleteClientFromEpoll(pipeFd);
        deleteClientFromEpoll(sock);
        return;
    }

    if (b_read == 0)
    {
        int status = 0;
        waitpid(_ClientsMap[pipeFd].pid, &status, WNOHANG);
        bool child_failed = (!WIFEXITED(status) || WEXITSTATUS(status) != 0);
        if (child_failed)
        {
            kill(_ClientsMap[pipeFd].pid, SIGKILL);
            SendErrorPage(sock, "500");
            deleteClientFromEpoll(pipeFd);
            deleteClientFromEpoll(sock);
            return;
        }
        if (send(sock, _ClientsMap[pipeFd].response.c_str(), _ClientsMap[pipeFd].response.size(), MSG_NOSIGNAL) < 0)
        {
            kill(_ClientsMap[pipeFd].pid, SIGKILL);
            deleteClientFromEpoll(pipeFd);
            deleteClientFromEpoll(sock);
            return;
        }
        deleteClientFromEpoll(pipeFd);
        deleteClientFromEpoll(sock);
        return;
    }

    if (!_ClientsMap[pipeFd].cgi_headers_parsed)
    {
        std::string data(buffer, b_read);

        size_t header_end = data.find("\r\n");
        if (header_end == std::string::npos || data.find("\r\n") == std::string::npos)
        {
            SendErrorPage(sock, "500");
            deleteClientFromEpoll(pipeFd);
            deleteClientFromEpoll(sock);
            return;
        }

        int status = 200;
        size_t status_pos = data.find("Status:");
        if (status_pos != std::string::npos)
            status = atoi(data.c_str() + status_pos + 7);

        std::string http =
            "HTTP/1.1 " + to_string(status) + "\r\n"
                                              "Connection: close\r\n";

        size_t body_start = header_end + 2;
        http += data.c_str() + body_start;
        _ClientsMap[pipeFd].response.append(http);
        _ClientsMap[pipeFd].cgi_headers_parsed = true;
        return;
    }
}
