#include "Server.hpp"

Server::Server(unsigned int host, short port) : _host(host), _port(port)
{
}

int	Server::setup(void)
{
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		return (-1);
	}
	std::cout << "socket: " << _listen_fd << std::endl;
	_addr.sin_family = AF_INET;
	//_addr.sin_addr.s_addr = htonl(_host);
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	if (bind(_listen_fd, (saddr*)&_addr, sizeof(_addr)) == -1)
	{
		std::cerr << "Error: bind" << std::endl;
		return (-2);
	}
	if (listen(_listen_fd, SERVER_BACKLOG) == -1)
	{
		std::cerr << "Error: bind" << std::endl;
		return (-3);
	}
	return (0);
}

void	Server::accept(void)
{
	int	socket = ::accept(_listen_fd, NULL, NULL); // should we take client addr ?
	if (socket != -1)
		fcntl(socket, F_SETFL, O_NONBLOCK);
	else
		std::cerr << "Error: accept" << std::endl;
	_socket = socket;
}

int	Server::recv(void)
{
	int			tmp = 0;
	int			bytes_read = 0;
	char		buf[BUFSIZE];
	std::string	request;

	while ((tmp = ::recv(_socket, buf, BUFSIZE - 1, 0)) > 0)
	{
		request += buf;
		bytes_read += tmp;
	}
	std::cout << "Request:" << std::endl << request << std::endl;
	return (bytes_read);
}

int	Server::send(void)
{
	return (1);
}

// getters

unsigned int	Server::getHost(void) const { return _host; }
short			Server::getPort(void) const { return _port; }
int				Server::getListenFd(void) const { return _listen_fd; }
int				Server::getSocket(void) const { return _socket; }