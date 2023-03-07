#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"

class	Response : public Request {

public:

	Response(void);
	Response(std::string request);
	virtual	~Response(void);

	void	process(void);
	void	handleGet(void);
	void	handlePost(void);
	void	handleDelete(void);

	bool	isCgi(void);

	void		createHeaders(void);
	void		appendHeaders(std::string & str);
	int			readStaticPage(void);
	int			readDefaultPage(int code);
	std::string	renderString(void);

private:

	std::string	_status_code;
	std::string	_status_text;
	int			_code;
	
	std::map<std::string, std::string>	_response_headers;

	std::string	_path;
	std::string	_response_body;

};

#endif
