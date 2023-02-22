#include "WebServer.hpp"

WebServer::WebServer(std::string const & config_file)
{
	_config.parse(config_file);
}

WebServer::~WebServer(void)
{
}


void	WebServer::handleResponse(void)
{
}

void	WebServer::handleRequest(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		int	socket = _servers[i].getSocket();
		if (!FD_ISSET(socket, &_read))
			continue ;

		// todo recv request
	}
}

void	WebServer::handleConnection(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		if (!FD_ISSET(_servers[i].getListenFd(), &_read))
			continue ;

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
	while (1)
	{
		int	ret = 0;
		while (ret == 0)
		{
			_read = _current;
			FD_ZERO(&_write);
			// deal with _w_set

			ret = select(_max_fd + 1, &_read, &_write, NULL, &timeout);
		}
		if (ret > 0)
		{
			handleResponse();
			handleRequest();
			handleConnection();
		}
		else
		{
			//todo error
		}
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
