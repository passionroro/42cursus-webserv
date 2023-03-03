#ifndef WEB_SERV_HPP
# define WEB_SERV_HPP

class	WebServ {

public:

	WebServ(std::string const & config_file);
	~WebServ(void);

	void	run(void);
	void	handleResponse(void);
	void	handleRequest(void);
	void	handleConnection(void);

private:

	Config	_config;
	int		_max_fd;
	fd_set	_current;
	fd_set	_read;
	fd_set	_write;

	struct timeval	timeout;


private:
	WebServ(void);
	WebServ(WebServ const & src);
	WebServ&	operator=(WebServ const & rhs);

};

#endif
