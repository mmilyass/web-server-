/* ************************************************************************** */
/*                    file: Request.cpp                                       */
/*                    by: ybounite <ybounite@student.1337.ma>                 */
/*                                                                            */
/*                    Created: 2025/11/16 09:42 by ybounite                   */
/*                    Updated: 2025/11/16 09:42 by ybounite                   */
/* ************************************************************************** */

#include "Request.hpp"

const std::string &Request::getMethod(void) const { return _Method; }
const std::string &Request::getUri(void) const { return _URI; }
const std::string &Request::getHTTPversion(void) const { return _Protocol; }
const std::map<std::string, std::string> &Request::getHeaders(void) const { return _Headers; }
const std::string &Request::getBody(void) const { return _Body; }
const std::string &Request::getPath(void) const { return _Path; }
const std::string &Request::getPathInfo(void) const { return _PathInfo; }
const std::string &Request::getQueryString(void) const { return _QueryString; }

void		Request::setHeader(const std::string &key, const std::string &value) {
	_Headers[key] = value;
}

/* my library */
bool is_not_space(char c) {
	return !std::isspace(static_cast<unsigned char>(c));
}

void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), is_not_space));
}

void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), is_not_space).base(), s.end());
}

void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}
/* my library */

std::string	Request::extractPathInfo(const std::string &uri) const
{
	//std::string scriptExt[] = {"py", "php", "sh", "pl", "rb", "lua"};
	size_t dotPos = uri.find_last_of('.');
	if (dotPos == std::string::npos)
		return "";

	size_t endOfScript = uri.find('/', dotPos);
	if (endOfScript == std::string::npos)
		return "";
	
	return uri.substr(endOfScript);
}

bool	Request::is_ValidRequest() const {
    return !_Method.empty() && !_URI.empty() && !_Protocol.empty();
}

void	Request::parseRequestLine(const std::string &line)
{
	std::istringstream ss(line);
	ss >> _Method >> _URI >> _Protocol;

	size_t queryPos = _URI.find('?');
	if (queryPos != std::string::npos) {
		_QueryString = _URI.substr(queryPos + 1);
		_URI = _URI.substr(0, queryPos);
	}

	_PathInfo = extractPathInfo(_URI);
	if (!_PathInfo.empty())
		_URI = _URI.substr(0, _URI.size() - _PathInfo.size());
	_Path = _URI;
	if (!is_ValidRequest()) {
		status = enMISSING_LINE;
		throw std::runtime_error("invalid or missing request line");
	}
}

void	Request::ParseHeaders( std::istringstream &stream ) {

	std::string	line;
	while (std::getline(stream, line)) {

		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);
		if (line.empty())
			break ;

		size_t	pos = line.find(":");
		if (pos == std::string::npos) {
			status = enMISSING_HEADER;
			throw std::runtime_error("invalid or missing request header");
		}
		std::string header_name = line.substr(0, pos);
		std::string header_value = line.substr(pos + 1);
		trim(header_name);
		trim(header_value);
		_Headers[header_name] = header_value;
	}
}

void	Request::ParseChunked( std::istringstream &stream ){

	std::string	ChunkSizeStr;
	size_t		TotalBytesRead = 0;
	while (std::getline(stream, ChunkSizeStr)) {
		size_t	ChunkSize = std::strtoul(ChunkSizeStr.c_str(), NULL, 16);
		if (ChunkSize == 0)
			break ;
		std::vector<char> ChunkData(ChunkSize);
		stream.read(ChunkData.data(), ChunkSize);
		_Body.append(ChunkData.begin(), ChunkData.end());
		TotalBytesRead += ChunkSize;
	}
}

void	Request::ParseBody( std::istringstream &stream ) {

	std::string	&ContentLength = _Headers["Content-Length"];
	if (ContentLength.empty()) {
		std::string	&TransferEncoding = _Headers["Transfer-Encoding"];
		if (TransferEncoding == "chunked") {
			ParseChunked(stream);
			return ;
		}
		return ;
	}
	size_t	len = std::atoi(ContentLength.c_str());
	if (len == 0)
		return ;
	_Body.resize(len);
	stream.read(&_Body[0], len);
	if (stream.gcount() < static_cast<std::streamsize>(len)) {
		status = enMISSING_BODY;
		throw std::runtime_error("Incomplete body data received");
	}
}

void Request::createNewSession(ServerConfig &config)
{
	std::string id = generateSessionId();

	config.sessions[id]["username"] = "Soufiane";

	setHeader("Set-Cookie", "session_id=" + id + "; HttpOnly; Path=/; Max-Age=30");
	cookies["session_id"] = id;
}

void	Request::CreateSessioncookies()
{
	if (_URI == "/pages/login.html" || _URI == "/pages/register.html") {
		std::string Id;
		if (cookies.count("session_id")) {
			Id = getHeader("Set-Cookie");
			Id = Id.substr(Id.find("=") + 1, 16);
		}
		else {
			createNewSession(_config.servers[0]);
		}
	}
}

void	Request::handleRequest(std::string &raw)
{
	try{
		std::istringstream	stream(raw);
		std::string			line;

		if (!std::getline(stream, line)) throw std::runtime_error("invalid request");

		if (!line.empty() && line[line.length() - 1] == '\r') line.erase(line.length() - 1);
		parseRequestLine(line);
		ParseHeaders(stream);
		ParseBody(stream);
		CreateSessioncookies();
		Msg::info(_Method +" "+ _URI);
		this->status = enVALID;
	}
	catch(std::exception &e) { std::cerr << e.what() << std::endl; }
}

std::string Request::getHeader(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = _Headers.find(key);
	if (it != _Headers.end())
		return it->second;
	return "";
}

size_t Request::getContentLength() const
{
	std::string len = getHeader("Content-Length");
	if (len.empty())
		return 0;
	return std::atoi(len.c_str());
}

void	Request::ParseCookies()
{
    std::string cookieHeader = getHeader("Cookie");
    if (cookieHeader.empty())
        return;

    std::stringstream ss(cookieHeader);
    std::string token;

    while (std::getline(ss, token, ';'))
    {
        size_t pos = token.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = token.substr(0, pos);
        std::string value = token.substr(pos + 1);

        // trim spaces
        while (!key.empty() && key[0] == ' ')
            key.erase(0, 1);

        cookies[key] = value;
    }
}
