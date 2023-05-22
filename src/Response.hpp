#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include "Cgi.hpp"

class	Response : public Request {

public:

	Response(void);
	Response(std::string request, Server& server_conf);
	virtual	~Response(void);

	void	getHeaders(void);

	void	createHeaders(void);

	std::string	buildPath(void);

	void		appendHeaders(std::string & str);
	void		cgi(Server& server_conf);
	void		directoryListing(void);
	int			readStaticPage(void);

	std::string	getStatusText(void);

	std::string	renderString(void);

	void	getContentType(void);
	void	getContentLength(void);
	
	void 	postMethod();
	void 	deleteMethod();
	
	
	
	std::string&	getResponseHead(void)		{ return _response_head; }

private:

	std::string	_status_code;
	std::string	_status_text;
	
	MapStr      _response_headers;

	std::string	_response_head;
	std::string	_response_body;

};

bool	comp(struct dirent x, struct dirent y);

#endif
