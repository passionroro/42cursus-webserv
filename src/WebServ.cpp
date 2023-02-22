#include "WebServ.hpp"

WebServ::WebServ(std::string const & config_file)
{
}

WebServ::~WebServ(void)
{
}


void	WebServ::handleResponse(void)
{
}

void	WebServ::handleRequest(void)
{
	for (int i = 0 ; i < _servers.size() ; i++)
	{
		int	socket = _servers[i]._socket;
		if (!FD_ISSET(socket, &_read))
			continue ;

		// todo recv request
	}
}

void	WebServ::handleConnection(void)
{
	for (int i = 0 ; i < _servers.size() ; i++)
	{
		if (!FD_ISSET(_servers[i]._listen_fd, &_read))
			continue ;

		_servers[i].accept();
		int	socket = _servers[i]._socket;
		if (socket != -1)
		{
			FD_SET(socket, &_current);
			_servers.addback(); // ok a voir du coup
			if (socket > _max_fd)
				_max_fd = socket;
		}
		break ;
	}
}

void	WebServ::run(void)
{
	while (1)
	{
		int	ret = 0;
		while (ret == 0)
		{
			_read = _current;
			FD_ZERO(&_write);
			// deal with _w_set

			ret = select(_max_fd + 1, &read, &write, NULL, &timeout);
		}
		if (ret > 0)
		{
			handle_write();
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

WebServ::WebServ(void)
{
}

WebServ::WebServ(WebServ const & src)
{
}

WebServ&	WebServ::operator=(WebServ const & rhs)
{
}
