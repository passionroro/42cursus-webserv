#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"
# include "config/Config.hpp"


class Request {

public:

	Request();
	Request(std::string request, Server& server_conf);
	virtual ~Request();
	
	int 	parseRequest(std::string &Request, Server& conf);
//	bool	check_path(std::string s);
	int 	parseHeaders(std::string &Request);

	std::string	buildPath(void);

	std::string getStatus() const;
	std::string getBody() const;
	std::string getRequestPath() const;
	void 		checkMethod();
	void 		checkPath();
	void		setStatus(std::string statusCode);

	void		checkHost(Server& conf);

	void		printHeaders(void);

	std::map<std::string, std::string> getRequestHeaders(void) { return _request_headers; }

protected:

	Locations			_locations;
	std::string			_path;
	Locations::iterator	_locIndex;

	bool	_isDir;

private:

	std::string 						_method;
	std::string 						_requestPath;
	std::string 						_version;
	std::map<std::string, std::string>	_request_headers;
	std::string							_request_body;
	std::string 						_status;


};
#endif
