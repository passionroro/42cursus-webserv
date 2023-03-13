#include "WebServer.hpp"

WebServer::WebServer(std::string const & config_file)
{
    // allocate config attributes inside Server
    (void) config_file;
	_servers.push_back(Server(4242, 8080));
	std::cout << "setup: host: " << _servers[0].getHost() << std::endl;
	std::cout << "port: " << _servers[0].getPort() << std::endl;
	_servers.push_back(Server(8484, 12000));
	std::cout << "setup: host: " << _servers[1].getHost() << std::endl;
	std::cout << "port: " << _servers[1].getPort() << std::endl;
}

WebServer::~WebServer(void)
{
}

int	WebServer::setup(void)
{
	_max_fd = 0;
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		if (_servers[i].setup() < 0)
			return (-1);
	}
	FD_ZERO(&_current_read);
	FD_ZERO(&_current_write);
	FD_ZERO(&_read);
	FD_ZERO(&_write);
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		int	fd = _servers[i].getListenFd();
		FD_SET(fd, &_current_read);
		if (fd > _max_fd)
			_max_fd = fd;
	}
	return (0);
}

void	WebServer::handleResponse(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		//std::cout << "try response" << std::endl;
		int	socket = _servers[i].getSocket();
		if (!FD_ISSET(socket, &_write))
			continue ;
		std::cout << "handle response" << std::endl;

		_servers[i].send(); // en vrai meme chose 0 ou -1, a verifier
		FD_CLR(socket, &_current_write);
		FD_CLR(socket, &_current_read);
		_servers[i].close();
		break ;
	}
}

void	WebServer::handleRequest(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		//std::cout << "try request" << std::endl;
		int	socket = _servers[i].getSocket();
		if (!FD_ISSET(socket, &_read))
			continue ;
		std::cout << "handle request" << std::endl;

		// todo recv request
		if (!_servers[i].recv())
		{
			//_servers[i]._response(_servers[i]._request);
			FD_SET(socket, &_current_write);
		}
		else
		{
			FD_CLR(socket, &_current_read);
		}
		break ;
	}
}

void	WebServer::handleConnection(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		//std::cout << "try connection" << std::endl;
		if (!FD_ISSET(_servers[i].getListenFd(), &_read))
			continue ;
		std::cout << "handle connection" << std::endl;

		_servers[i].accept();
		int	socket = _servers[i].getSocket();
		if (socket != -1)
		{
			FD_SET(socket, &_current_read);
			if (socket > _max_fd)
				_max_fd = socket;
		}
		break ;
	}
}

void	WebServer::run(void)
{
	std::cout << "Loop start" << std::endl;
	while (1)
	{
		int	ret = 0;
		//int	errno;
		while (ret == 0)
		{
			_timeout.tv_sec = 3;
			_timeout.tv_usec = 0;
			_read = _current_read;
			_write = _current_write;

			//std::cout << "max_fd: " << _max_fd << std::endl;
			ret = select(_max_fd + 1, &_read, &_write, NULL, &_timeout);
			//analyzeSets();
			std::cout << "time" << std::endl;
		}
		if (ret > 0)
		{
			std::cout << "New event!" << std::endl;
			handleRequest();
			handleConnection();
			handleResponse();
			ret = 0;
		}
		else
		{
			std::cerr << "Error: select failed ! " << strerror(errno) << std::endl;
		}
		//usleep(500000);
	}
}




void	WebServer::analyzeSets(void)
{
	std::cout << "Current_read: ";
	for (unsigned long i = 0 ; i < 1000 ; i++)
	{
		if (FD_ISSET(i, &_current_read))
			std::cout << i << ", ";
	}
	std::cout << std::endl;

	std::cout << "Read: ";
	for (unsigned long i = 0 ; i < 1000 ; i++)
	{
		if (FD_ISSET(i, &_read))
			std::cout << i << ", ";
	}
	std::cout << std::endl;
	
	std::cout << "Current_write: ";
	for (unsigned long i = 0 ; i < 1000 ; i++)
	{
		if (FD_ISSET(i, &_current_write))
			std::cout << i << ", ";
	}
	std::cout << std::endl;
	
	std::cout << "Write: ";
	for (unsigned long i = 0 ; i < 1000 ; i++)
	{
		if (FD_ISSET(i, &_current_write))
			std::cout << i << ", ";
	}
	std::cout << std::endl;
}




// private

WebServer::WebServer(void)
{
}

WebServer::WebServer(WebServer const & src)
{
	(void)src;
}

WebServer&	WebServer::operator=(WebServer const & rhs)
{
	(void)rhs;
	return (*this);
}
