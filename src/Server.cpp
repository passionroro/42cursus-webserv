#include "Server.hpp"


int	Server::setup(void)
{
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		return (-1);
	}
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(_host);
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

// getters

unsigned int	Server::getHost(void) const { return _host; }
short			Server::getPort(void) const { return _port; }
int				Server::getListenFd(void) const { return _listen_fd; }
int				Server::getSocket(void) const { return _socket; }
