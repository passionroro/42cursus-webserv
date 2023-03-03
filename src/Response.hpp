#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

class	Response {

public:

	Response(void);
	virtual	~Response(void);

	void	createHeaders(void);

	void		appendHeaders(std::string & str);
	int			readStaticPage(void);
	std::string	renderString(void);

private:

	std::map<std::string, std::string>	_headers;

	std::string	_path;
	std::string	_page;

};

#endif
