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
	void 	parseBody(std::string &Request);
	
	
	std::string	buildPath(void);

	std::string getStatus() const;
	std::string getBody() const;
	std::string getRequestPath() const;
	void 		checkMethod();
	void 		checkRedirection(Server& conf);
	void 		checkPath();
	void		setStatus(std::string statusCode);

	void		checkHost(Server& conf);

	void		printHeaders(void);
	std::map<std::string, std::string> getRequestHeaders(void) { return _requestHeaders; }

	std::string&	getPath(void)		{ return _path; }
	std::string&	getRequestPath(void)		{ return _requestPath; }
	std::string&	getRequestBody(void)		{ return _requestBody; }
	std::string&	getMethod(void)		{ return _method; }
	std::string&	getCgiBin(void)		{ return _cgiBin; }

protected:

	Locations			_locations;
	std::string			_path;
	Locations::iterator			_locIndex;
	std::string 		_method;
	bool				_isDir;
	std::string         _requestBody;
	std::map<std::string, std::string>	_requestHeaders;
	std::string			_newURL;
	std::string			_cgiBin;

	bool		_cgiDone;


private:

	std::string 						_requestPath;
	std::string 						_version;
	std::string 						_status;
	std::vector<std::string>			_disabledMethod;
	
//	bool								_isChunked;

};
#endif
