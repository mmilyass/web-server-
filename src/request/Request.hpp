/* ************************************************************************** */
/*                    file: Request.hpp                                       */
/*                    by: ybounite <ybounite@student.1337.ma>                 */
/*                                                                            */
/*                    Created: 2025/11/16 09:36 by ybounite                   */
/*                    Updated: 2025/11/16 09:36 by ybounite                   */
/* ************************************************************************** */

# pragma once
# include "../../includes/Webserv.hpp"

/* ************************************************************************** */
/*                          Request Class                                     */
/* ************************************************************************** */

class Request {
public:

	enum	enStatusRequest{ enVALID, enINVALID, enMISSING_LINE, enMALFORMED_LINE, enMISSING_HEADER, enMISSING_BODY};

	Request() {}

	Request(std::string &raw, Config &ConfigFile) : _PathInfo(""), _QueryString(""), _config(ConfigFile)
	{ this->handleRequest(raw); }

	Request(const Request &Other) : status(Other.status), cookies(Other.cookies),
	isQueryString(Other.isQueryString), _Method(Other._Method), _URI(Other._URI),
	_Protocol(Other._Protocol), _Headers(Other._Headers), _Body(Other._Body),
	_Path(Other._Path), _PathInfo(Other._PathInfo), _QueryString(Other._QueryString), _config(Other._config) {}

	~Request(void) {}

	enStatusRequest						status;
	std::map<std::string, std::string>	cookies;
	bool								isQueryString;

	const std::string							&getMethod( void ) const;
	const std::string							&getUri( void ) const;
	const std::string							&getHTTPversion( void ) const;
	const std::string							&getQueryString( void ) const;
	const std::map<std::string, std::string>	&getHeaders( void ) const;
	const std::string							&getBody( void ) const;
	const std::string							&getPath( void ) const;
	const std::string							&getPathInfo( void ) const;

	void								setHeader(const std::string &key, const std::string &value);
	void								handleRequest( std::string &raw );
	void								CreateSessioncookies();
	void								createNewSession(ServerConfig &config); // 

	void								parseRequestLine( const std::string &line );
	void								ParseHeaders( std::istringstream &stream );
	void								ParseBody( std::istringstream &stream );
	void								ParseChunked( std::istringstream &stream ); //
	std::string							getHeader(const std::string &key) const;
	size_t								getContentLength() const;
	std::string							extractPathInfo(const std::string &uri) const;
	//
	void					ParseCookies();

private:
	std::string							_Method;
	std::string							_URI;
	std::string							_Protocol;
	std::map<std::string, std::string>	_Headers;
	std::string							_Body;
	std::string							_Path;
	std::string							_PathInfo;
	std::string							_QueryString;

	bool								is_ValidRequest() const;

protected:
	Config								_config;
};
