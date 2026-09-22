/* ************************************************************************** */
/*                    file: Response.hpp                                      */
/*                    by: ybounite <ybounite@student.1337.ma>                 */
/*                                                                            */
/*                    Created: 2025/11/29 16:02 by ybounite                   */
/*                    Updated: 2025/11/29 16:02 by ybounite                   */
/* ************************************************************************** */

# pragma once
# include "../../includes/Webserv.hpp"

/* ************************************************************************** */
/*                          Response Class                                    */
/* ************************************************************************** */

class	Request;

class Response : protected Request {
public:

	Response( const Request &req ) : Request(req), StatusCode(200),
		Headers(), Fd(-1), BodySize(0), Header(""), Body(""),
		uri(""), FilePath(""), isCGI(false) {}
		
	Response(const Response &Other) : Request(Other), cgiInfo(Other.cgiInfo), 
		StatusCode(Other.StatusCode), Headers(Other.Headers), Fd(Other.Fd),
		BodySize(Other.BodySize), Header(Other.Header), Body(Other.Body), 
		uri(Other.uri), FilePath(Other.FilePath), isCGI(Other.isCGI) {}

	~Response() {}

	stCgiInfo								cgiInfo;
	short									StatusCode;
	std::map<std::string, std::string>		Headers;
	int										Fd;
	ssize_t									BodySize;
	std::string								Header;
	std::string								Body;
	std::string								uri;
	std::string								FilePath;
	bool									isCGI;
	
	std::string					BuildHeaderResponse();
	void						setStatusCode(short code);
	void						setHeader(const std::string &key, const std::string &value);
	void						setBody(const std::string &body);
	std::string					guessContentType(const std::string &path);
	std::string					getStatusMessage(short code) const;

};
