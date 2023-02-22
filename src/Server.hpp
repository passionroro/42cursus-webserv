#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Config.hpp"

# define SERVER_BACKLOG 1000 // to define in header webserv.hpp i think

typedef struct sockaddr_in saddr_in;
typedef struct sockaddr saddr;

class	Server {

public:

	Server(unsigned int host, short port); // maybe ?

	void	accept(void);
	int		setup(void);

	unsigned int	getHost(void) const;
	short			getPort(void) const;
	int	getListenFd(void) const;
	int	getSocket(void) const;

private:

	unsigned int	_host;
	short			_port;

	int			_listen_fd;
	int			_socket;

	saddr_in	_addr;

private:

	Server(void);

};

#endif
