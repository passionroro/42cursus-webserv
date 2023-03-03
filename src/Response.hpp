#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"

class	Response {

public:

	Response(void);
	Response(Request const & request);
	virtual	~Response(void);

	void	getHeaders(void);

	void	createHeaders(void);

	void		appendHeaders(std::string & str);
	int			readStaticPage(void);
	std::string	renderString(void);

private:

	std::string	_version;
	std::string	_status_code;
	std::string	_status_text;
	
	std::map<std::string, std::string>	_headers;

	std::string	_path;
	std::string	_body;

};

#endif
