#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Config.hpp"
# include "Response.hpp"

typedef struct sockaddr_in saddr_in;
typedef struct sockaddr saddr;

class	Server {

public:

    Server(unsigned int host, short port); // maybe ?
	Server(Object const & object);

    void	accept(void);
    int		setup(void);

    int	recv(void);
    int	send(void);

    void	close(void);

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

    Response	_response;

private:

    Server(void);

};

#endif
