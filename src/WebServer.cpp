#include "WebServer.hpp"

WebServer::WebServer(std::string const & config_file)
{
	_config.parse(config_file);
	_servers.push_back(Server(4242, 8080));
	std::cout << "setup: host: " << _servers[0].getHost() << std::endl;
	std::cout << "port: " << _servers[0].getPort() << std::endl;
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
	FD_ZERO(&_current);
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		FD_SET(_servers[i].getListenFd(), &_current);
		if (_servers[i].getListenFd() > _max_fd)
			_max_fd = _servers[i].getListenFd();
	}
	return (0);
}

void	WebServer::handleResponse(void)
{
	//std::cout << "handle response" << std::endl;
}

void	WebServer::handleRequest(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		int	socket = _servers[i].getSocket();
		if (!FD_ISSET(socket, &_read))
			continue ;
		std::cout << "handle request" << std::endl;

		// todo recv request
		_servers[i].recv();
	}
}

void	WebServer::handleConnection(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		if (!FD_ISSET(_servers[i].getListenFd(), &_read))
			continue ;
		std::cout << "handle connection" << std::endl;

		_servers[i].accept();
		int	socket = _servers[i].getSocket();
		if (socket != -1)
		{
			FD_SET(socket, &_current);
			//_servers.addback(); // ok a voir du coup
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
		while (ret == 0)
		{
			_read = _current;
			FD_ZERO(&_write);
			// deal with _w_set

			//ret = select(_max_fd + 1, &_read, &_write, NULL, &timeout);
			(void)_timeout;
			//std::cout << "max_fd: " << _max_fd << std::endl;
			ret = select(_max_fd + 1, &_read, &_write, NULL, NULL);
		}
		if (ret > 0)
		{
			//std::cout << "select worked! ret: " << ret << std::endl;
			handleResponse();
			handleRequest();
			handleConnection();
			ret = 0;
		}
		else
		{
			//todo error
		}
		usleep(500000);
	}
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
