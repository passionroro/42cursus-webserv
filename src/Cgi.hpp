#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"

class Cgi {

public:
	
	Cgi(Request & request);
	virtual ~Cgi(void);

	void	initEnv(void);
	void	execute(void);

	char	**getEnvv(void) const;
	
private:

	Request&	_r;
	std::map<std::string, std::string>	_env;
	std::string	_body;

private:

	Cgi(void);

};

#endif
