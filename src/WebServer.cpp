#include "WebServer.hpp"

WebServer::WebServer(Config default_cfg, Config config) {

    Object default_object = default_cfg.getData().getArray()["servers"].getObject().front();

    if (config.overwrite) {

        std::map<std::string, Array>	map_array = config.getData().getArray();
        Array                           array = map_array["servers"];

        std::vector<Object>::iterator it;
        for (it = array.getObject().begin() ; it != array.getObject().end() ; it++) {
            _servers.push_back(Server(default_object, *it));
        }
    }

    else {
        _servers.push_back(Server(default_object));
    }

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
    	if (_servers[i].getSocket().empty())
        	continue;
		int	socket = _servers[i].getSocket().front();
		if (!FD_ISSET(socket, &_write))
			continue ;

		if (_servers[i].send(socket) > 0)
			continue ;
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

    	if (_servers[i].getSocket().empty())
      		continue;
		int	socket = _servers[i].getSocket().front();
		if (!FD_ISSET(socket, &_read))
			continue ;

		if (_servers[i].recv(socket) > 0)
			continue ;
		FD_SET(socket, &_current_write);
		FD_CLR(socket, &_current_read);
		break ;
	}
}

void	WebServer::handleConnection(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		if (!FD_ISSET(_servers[i].getListenFd(), &_read))
			continue ;

		_servers[i].accept();
		int	socket = _servers[i].getSocket().back();
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
	while (1)
	{
		int	ret = 0;

		while (ret == 0)
		{
			_read = _current_read;
			_write = _current_write;

			ret = select(_max_fd + 1, &_read, &_write, NULL, NULL);
		}
		if (ret > 0)
		{
			handleConnection();
			handleRequest();
			handleResponse();
			ret = 0;
		}
		else
		{
			std::cerr << "Error: select failed ! " << strerror(errno) << std::endl;
			sleep(1);
		}
	}
}




void	WebServer::analyzeSets(void)
{
	std::cout << "------------------------" << std::endl;
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

	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		_servers[i].printSocket();
	}
	std::cout << "------------------------" << std::endl;
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
