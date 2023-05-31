#include "Server.hpp"
#include <iomanip>

/* CONSTRUCTORS */
// Server constructor when no config file is given
Server::Server(Object & default_obj) {

    assignDefaultConfig(default_obj);

    std::cout << "Server listening on " << _address[0] << ":" << _port << std::endl;

}

// Server constructor if a config file is given
Server::Server(Object & default_obj, Object & object) {

    assignDefaultConfig(default_obj);
    if (assignNewConfig(object))
	{
		std::cout << "Bad config file!" << std::endl;
		exit(1);
	}

    std::cout << "Server listening on " << _address[0] << ":" << _port << std::endl;

}

/* FUNCTIONS */
int	Server::setup(void)
{
    _listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_fd == -1)
    {
        std::cerr << "Error: socket" << std::endl;
        return (-1);
    }
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
    _socket.push(socket);
}

int	Server::recv(int socket)
{
    int			tmp = 0;
    int			bytes_read = 0;
    char		buf[BUFSIZE];
    std::string	request;

    while ((tmp = ::recv(socket, buf, BUFSIZE - 1, 0)) > 0)
    {
		buf[tmp] = '\0';
       request += std::string(buf, tmp);
		bytes_read += tmp;
    }
    buf[BUFSIZE - 1] = '\0';
    if (bytes_read == 0)
    {
        std::cerr << "t-as rien lu bro" << std::endl;
        return (1);
    }
    if (bytes_read == -1)
    {
        std::cerr << "Error: recv: " << strerror(errno) << std::endl;
        return (-1);
    }

    _response = Response(request, *this);
  
	std::string	print = request.substr(0, request.find('\n'));
	std::cout << "Request: " << BLUE << print << DEFAULT << std::endl;

    return (0);
}

int	Server::send(int socket)
{
    std::string	str = _response.renderString();

	std::string	print = _response.getResponseHead().substr(0, _response.getResponseHead().find('\n'));
	std::cout << "Request: " << LGREEN << print << DEFAULT << std::endl;

    if ((::send(socket, str.c_str(), str.size(), 0)) < 0)
        return (-1);
    else
        return (0);
}

void	Server::close(void)
{
    if (_socket.size() && _socket.front() > 0)
	{
        ::close(_socket.front());
		_socket.pop();
	}
}

void	Server::printSocket(void)
{
	if (_socket.empty())
		std::cout << "No socket" << std::endl;
	else
		std::cout << "front: " << _socket.front() << std::endl;
}

/* CFG UTILS */
void Server::assignDefaultConfig(Object &object) {

    _server_name = object.getString()["server_name"];
    _port = object.getInt()["port"];
    _clt_body_size = object.getInt()["client_max_body_size"];
    _auto_index = object.getBool()["auto_index"];

    _address = object.getArray()["address"].getString();
    _disabled_methods = object.getArray()["disabled_methods"].getString();

    _error_pages = object.getObject()["error_pages"].getString();

    std::vector<Object>             tmp_location;
    std::vector<Object>::iterator   it;

    tmp_location = object.getArray()["locations"].getObject();
    for (it = tmp_location.begin() ; it != tmp_location.end() ; it++) {
        _locations.push_back(it->getString());
    }
	
	//	CGI BLOCK
	{
		std::vector<Object>             tmp_cgi;
		std::vector<Object>::iterator   it;

		tmp_cgi = object.getArray()["cgi"].getObject();
		for (it = tmp_cgi.begin() ; it != tmp_cgi.end() ; it++) {
			_cgi.push_back(it->getString());
		}
	}

}

bool Server::assignNewConfig(Object & object)
{
    if (object.getString().find(std::string("server_name")) != object.getString().end())
        _server_name = object.getString()["server_name"];
    if (object.getInt().find(std::string("port")) != object.getInt().end())
        _port = object.getInt()["port"];
    if (object.getInt().find(std::string("client_max_body_size")) != object.getInt().end())
        _clt_body_size = object.getInt()["client_max_body_size"];
    if (object.getBool().find(std::string("auto_index")) != object.getBool().end())
        _auto_index = object.getBool()["auto_index"];
    if (object.getArray().find(std::string("address")) != object.getArray().end())
        _address = object.getArray()["address"].getString();
    if (object.getArray().find(std::string("disabled_methods")) != object.getArray().end())
        _disabled_methods = object.getArray()["disabled_methods"].getString();

    //error_pages
    if (object.getObject().find(std::string("error_pages")) != object.getObject().end()) {

        MapStr  new_error_page = object.getObject()["error_pages"].getString();
        MapStr::iterator it;

        for (it = _error_pages.begin(); it != _error_pages.end(); it++) {
            MapStr::iterator search;
            search = new_error_page.find(std::string(it->first));
            if (search != new_error_page.end())
                it->second = search->second;
        }

    }

    //location block
    if (object.getArray().find(std::string("locations")) != object.getArray().end())
	{
        std::vector<Object>             new_loc;
        std::vector<Object>::iterator   it;

        new_loc = object.getArray()["locations"].getObject();

        // Iterate through every location block from the config file.
        for (it = new_loc.begin() ; it != new_loc.end() ; it++)
		{

            MapStr              new_content;
            MapStr::iterator    path;
            Locations::iterator def_it;

            new_content = it->getString();
            path = new_content.find(std::string("path"));

            if (path != new_content.end())
			{
                bool new_location = true;

                // Iterate through every DEFAULT location block.
                for (def_it = _locations.begin(); def_it != _locations.end(); def_it++)
				{
                    MapStr::iterator    new_path;

                    new_path = def_it->find(std::string("path"));
                    if (new_path == def_it->end())
                        break ;

                    // If both location block share the same path, overwrite them.
                    else if (new_path->second == path->second)
					{
                        MapStr::iterator    search;
                        for (search = new_content.begin(); search != new_content.end(); search++)
						{
                            new_path = def_it->find(std::string(search->first));
                            if (new_path != def_it->end())
                                new_path->second = search->second;
                        }
                        new_location = false;
                        break ;
                    }
                }
                if (new_location)
                    _locations.push_back(new_content);
            }
            else
                std::cerr << "cfg: location block needs a path: no creation." << std::endl;

        }
	}

	//	REDIRECTION BLOCK
	{
		std::vector<Object>             tmp_redirection;
		std::vector<Object>::iterator   it;

		tmp_redirection = object.getArray()["redirection"].getObject();
		for (it = tmp_redirection.begin() ; it != tmp_redirection.end() ; it++) {
			_redirection.push_back(it->getString());
		}

		Redirection::const_iterator	it2;

		for (it2 = _redirection.begin() ; it2 != _redirection.end() ; it2++)
		{
			try
			{
				it2->at("old_url");
				it2->at("new_url");
				it2->at("type");
			}
			catch (std::exception& e)
			{
				std::cerr << "Redirection badly defined" << std::endl;
				return true;
			}
		}
	}
    
	//	CGI BLOCK
	{
		std::vector<Object>             tmp_cgi;
		std::vector<Object>::iterator   it;

		tmp_cgi = object.getArray()["cgi"].getObject();
		for (it = tmp_cgi.begin() ; it != tmp_cgi.end() ; it++) {
			_cgi.push_back(it->getString());
		}
	}
	return false;
}

/* ACCESSORS */
unsigned int	Server::getHost() const { return _host; }
int				Server::getListenFd() const { return _listen_fd; }
std::queue<int>				Server::getSocket() const { return _socket; }

std::vector<std::string> Server::getAddress() const { return _address; }
std::vector<std::string> Server::getDisabledMethods() const { return _disabled_methods; }

MapStr      Server::getErrorPages() const { return _error_pages; }
std::string Server::getServerName() const { return _server_name; }
bool        Server::getAutoIndex() const { return _auto_index; }
int         Server::getClientBodySize() const { return _clt_body_size; }
int         Server::getPort() const { return _port; }
