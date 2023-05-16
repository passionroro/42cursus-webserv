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
		//std::cout << "socket handle response: " << socket << std::endl;
		//analyzeSets();
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

    if (_servers[i].getSocket().empty())
      continue;
		int	socket = _servers[i].getSocket().front();
		//std::cout << "socket handle request: " << socket << std::endl;
		//analyzeSets();
		if (!FD_ISSET(socket, &_read))
			continue ;
		std::cout << "handle request" << std::endl;

		int	tmp = _servers[i].recv();

		if (tmp == 0)
		{
			std::cout << "socket into write: " << socket << std::endl;
			FD_SET(socket, &_current_write);
			//FD_CLR(socket, &_current_read);
		}
		/*else if (tmp == 1)
		{
			tmp = 0;
			continue ;
		}*/
		else
		{
			FD_CLR(socket, &_current_read);
			//FD_CLR(_servers[i].getListenFd(), &_current_read);
			_servers[i].close();
		}
		break ;
	}
}

void	WebServer::handleConnection(void)
{
	for (unsigned long i = 0 ; i < _servers.size() ; i++)
	{
		//analyzeSets();
		if (!FD_ISSET(_servers[i].getListenFd(), &_read))
			continue ;
		std::cout << "handle connection" << std::endl;

		_servers[i].accept();
		int	socket = _servers[i].getSocket().front();
		if (socket != -1)
		{
			std::cout << "socket into read: " << socket << std::endl;
			//analyzeSets();
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
			//analyzeSets();
			ret = 0;
		}
		else
		{
			std::cerr << "Error: select failed ! " << strerror(errno) << std::endl;
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
