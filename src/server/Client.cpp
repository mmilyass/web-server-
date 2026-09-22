#include "../../includes/Webserv.hpp"
#include "../response/Response.hpp"
#include "../request/RequestHandler.hpp"

void Server::addClientInEppol(int fd)
{
	int clientFd;
	clientFd = accept(fd, NULL, NULL);
	if (clientFd < 0)
	{
		std::cerr << "Cant accept Client" << std::endl;
		return;
	}
	addNblock(clientFd);
	epoll_event newClient;
	newClient.data.fd = clientFd;
	newClient.events = EPOLLIN;
	if (epoll_ctl(_epollInstance, EPOLL_CTL_ADD, clientFd, &newClient) < 0)
	{
		close(clientFd);
		SendErrorPage(clientFd, "500");
		return;
	}
	t_clients client;
	client.fd = clientFd;
	client.event = newClient;
	_ClientsMap[clientFd] = client;
}

void Server::readClientRequest(unsigned int clientFd)
{
	int bytesRead;
	char buffer[1024];
	bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	if (bytesRead == 0)
		return (deleteClientFromEpoll(clientFd));
	else if (bytesRead < 0)
		return;
	_ClientsMap[clientFd].request.append(buffer, bytesRead);
	size_t headerEnd = _ClientsMap[clientFd].request.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		return;
	if (contentLenght(_ClientsMap[clientFd].request) > _ClientsMap[clientFd].request.length() - headerEnd - 4)
		return;
	_ClientsMap[clientFd].last_activity = time(NULL);
	modifySockEvents(_epollInstance, clientFd);
}

void Server::sendHttpResponse(int clientFd)
{
	if (_ClientsMap[clientFd].CGIfd > -1)
		return;

	if (time(NULL) - _ClientsMap[clientFd].last_activity > 10)
	{
		std::cerr << "Client timeout, closing connection\n";
		deleteClientFromEpoll(clientFd);
		return;
	}
	if (_ClientsMap[clientFd].firstTime)
		return getReadInfos(clientFd);

	if (_ClientsMap[clientFd].clsResponse->isCGI)
		return;

	if (_ClientsMap[clientFd].clsResponse->Fd == -1)
		return errorSending(clientFd);

	ReadSend(clientFd);
}