/* ************************************************************************** */
/*                    file: RequestHandler.hpp                                */
/*                    by: ybounite <ybounite@student.1337.ma>                 */
/*                                                                            */
/*                    Created: 2025/11/29 16:09 by ybounite                   */
/*                    Updated: 2025/11/29 16:09 by ybounite                   */
/* ************************************************************************** */

# pragma once
# include "../../includes/Webserv.hpp"

class Response;

/* ************************************************************************** */
/*                          RequestHandler Class                              */
/* ************************************************************************** */

class RequestHandler {
public:
	RequestHandler(const Request &Req, const ServerConfig &Conf): req(Req), config(Conf) {}
	enum	enHttpMethod { HTTP_GET, HTTP_POST, HTTP_DELETE, HTTP_UNKNOWN };

	// Static handler - processes Request and returns Response
	Response		HandleMethod();
	short			getMethod( const std::string &method );
	Response		BuildErrorResponse( short code );
	size_t			GetBodySize(const std::string &path);

private:
	const Request		&req;
	const ServerConfig	&config;
	// Helper methods for different HTTP methods
	Response			handleGET();
	Response			handlePOST();
	Response			handleDELETE();

	// Utility functions
	std::string			readFile(const std::string &path);
	std::string			getErrorPage(int statusCode);
	std::string			_BuildFileSystemPath(const std::string &root, const std::string &uri);
	Response			_GenerateAutoindex(const std::string &dirPath);
	bool				_haseAllowed( std::vector<std::string> Methods, enHttpMethod AllowedMethod);
	Response			_Rdirect(LocationConfig loc);
	Response			_BuildCGIResponse(const std::string &path);

	bool					_ResourceExists( std::string &Path );
	std::string				_ResolveIndexFile(const std::string &path, const ServerConfig &server, const LocationConfig &loc);
	Response				serveFile(const std::string &path);
};
