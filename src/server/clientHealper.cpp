#include "../../includes/Webserv.hpp"
#include "../response/Response.hpp"
#include "../request/RequestHandler.hpp"

void Server::modifySockEvents(int epollfd, int fd)
{
	epoll_event newClient;
	newClient.data.fd = fd;
	newClient.events = EPOLLIN | EPOLLOUT;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &newClient);
}

void Server::deleteClientFromEpoll(unsigned int clientFd)
{
	epoll_ctl(_epollInstance, EPOLL_CTL_DEL, clientFd, NULL);
	close(clientFd);
	if (_ClientsMap[clientFd].clsResponse != NULL)
	{
		if (_ClientsMap[clientFd].clsResponse->Fd != -1)
			close(_ClientsMap[clientFd].clsResponse->Fd);
		delete _ClientsMap[clientFd].clsResponse;
	}
	_ClientsMap.erase(clientFd);
}

size_t Server::contentLenght(std::string header)
{
	size_t index = header.find("Content-Length:");
	if (index == std::string::npos)
		return 0;

	index += 15;

	while (index < header.length() && header[index] == ' ')
		index++;

	size_t endIndex = header.find("\r\n", index);
	if (endIndex == std::string::npos)
		return 0;

	std::string Clenght = header.substr(index, endIndex - index);
	return atoi(Clenght.c_str());
}

void Server::ifCGI(int fd)
{
	cgi Cgi;
	Cgi.CGIhandler(_ClientsMap[fd].clsResponse->cgiInfo);
	int pipeFd = Cgi._pipeFd;

	if (pipeFd < 0)
	{
		SendErrorPage(fd, "500");
		deleteClientFromEpoll(fd);
		return;
	}
	epoll_event events;
	events.data.fd = pipeFd;
	events.events = EPOLLIN;
	epoll_ctl(_epollInstance, EPOLL_CTL_ADD, pipeFd, &events);
	t_clients client;
	client.fd = fd;
	client.CGIfd = pipeFd;
	client.last_activity = time(NULL);
	client.pid = Cgi._Pid;
	_ClientsMap[pipeFd] = client;
	_ClientsMap[fd].event.events = 0;
	epoll_ctl(_epollInstance, EPOLL_CTL_MOD, fd, &_ClientsMap[fd].event);
	return;
}

void Server::getReadInfos(unsigned int fd)
{
	ssize_t sendBytes;
	Request req(_ClientsMap[fd].request, _data);
	RequestHandler ReqH(req, _data.servers[0]);
	_ClientsMap[fd].clsResponse = new Response(ReqH.HandleMethod());
	if (_ClientsMap[fd].clsResponse->isCGI == 1)
	{
		ifCGI(fd);
		return;
	}
	_ClientsMap[fd].response = _ClientsMap[fd].clsResponse->BuildHeaderResponse();
	sendBytes = send(fd, _ClientsMap[fd].response.c_str(), _ClientsMap[fd].response.length(), 0);
	if (sendBytes < 0)
	{
		SendErrorPage(fd, "500");
		return deleteClientFromEpoll(fd);
	}
	_ClientsMap[fd].firstTime = false;
}

void Server::errorSending(unsigned int fd)
{
	ssize_t sendBytes;
	if (!_ClientsMap[fd].clsResponse->Body.empty())
	{
		sendBytes = send(fd, _ClientsMap[fd].clsResponse->Body.c_str(),
						 _ClientsMap[fd].clsResponse->Body.length(), 0);
		if (sendBytes < 0)
			return deleteClientFromEpoll(fd);
	}
	deleteClientFromEpoll(fd);
}

void Server::ReadSend(unsigned int fd)
{
	char buffer[1024];
	ssize_t sendBytes;
	ssize_t b_read = read(_ClientsMap[fd].clsResponse->Fd, buffer, sizeof(buffer));
	if (b_read <= 0)
	{
		deleteClientFromEpoll(fd);
		return;
	}
	sendBytes = send(fd, buffer, b_read, 0);
	if (sendBytes < 0)
	{
		deleteClientFromEpoll(fd);
		return;
	}
	_ClientsMap[fd].bytesread += sendBytes;
	if (_ClientsMap[fd].bytesread >= _ClientsMap[fd].clsResponse->BodySize)
	{
		deleteClientFromEpoll(fd);
		_ClientsMap[fd].last_activity = time(NULL);
	}
}
