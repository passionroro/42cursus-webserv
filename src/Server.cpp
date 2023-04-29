#include "Server.hpp"

Server::Server(unsigned int host, short port) : _host(host), _port(port)
{
}

Server::Server(Object & object)
{
	// get default config here ? needed for each server


	getConfig(object);

}

void	Server::getConfig(Object & object)
{
	// add location(s)
	Array array = object.getArray()["locations"];

	for (std::vector<Object>::iterator it = array.getObject().begin() ;
		it != array.getObject().end() ; it++)
	{
		_locations.push_back(*it);
	}

	printLocations();


	_port = object.getInt()["port"];
	std::cout << "port = " << _port << std::endl;

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

    std::cout << "Webserv: recv" << std::endl;
    while ((tmp = ::recv(_socket, buf, BUFSIZE - 1, 0)) > 0)
    {
        request += buf;
        bytes_read += tmp;
    }
    buf[BUFSIZE - 1] = '\0';
    if (bytes_read == 0 || bytes_read == -1)
    {
        std::cerr << "Error: recv" << std::endl;
        return (-1);
    }
    _response = Response(request, _locations);
    std::cout << "Request:" << std::endl << request << std::endl;
    return (0);
}

int	Server::send(void)
{
    std::string	str = _response.renderString();

    std::cout << "Webserv: send" << std::endl;
    //std::cout << "Response:" << std::endl << str << std::endl;
    if ((::send(_socket, str.c_str(), str.size(), 0)) < 0)
        return (-1);
    else
        return (0);
}

void	Server::close(void)
{
    std::cout << "Webserv: close" << std::endl;
    if (_socket > 0)
        ::close(_socket);
}

// debug functions


void	Server::printLocations(void)
{
	for (unsigned long i = 0 ; i < _locations.size() ; i++)
	{
		std::cout << "location " << i + 1 << std::endl;
		std::cout << "path: " << _locations[i].getString()["path"] << std::endl;
		std::cout << "index: " << _locations[i].getString()["index"] << std::endl;
	}
}

// getters

unsigned int	Server::getHost(void) const { return _host; }
short			Server::getPort(void) const { return _port; }
int				Server::getListenFd(void) const { return _listen_fd; }
int				Server::getSocket(void) const { return _socket; }
