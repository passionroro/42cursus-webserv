#ifndef WEB_SERVER_HPP
# define WEB_SERVER_HPP

# include "webserv.hpp"
# include "Server.hpp"

class	WebServer {

public:

	WebServer(std::string const & config_file);
	~WebServer(void);

	int		setup(void);
	void	run(void);
	void	handleResponse(void);
	void	handleRequest(void);
	void	handleConnection(void);

	void	analyzeSets(void);

private:

//	Config	_config;
	int		_max_fd;
	fd_set	_current_read;
	fd_set	_current_write;
	fd_set	_read;
	fd_set	_write;

	struct timeval	_timeout;

	std::vector<Server>	_servers;


private:
	WebServer(void);
	WebServer(WebServer const & src);
	WebServer&	operator=(WebServer const & rhs);

};

#endif
