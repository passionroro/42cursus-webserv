#include "Server.hpp"

/* CONSTRUCTORS */
// Server constructor when no config file is given
Server::Server(Object & default_obj) {

    std::cout << "Server default constructor." << std::endl;
    assignDefaultConfig(default_obj);

}

// Server constructor if a config file is given
Server::Server(Object & default_obj, Object & object) {

    std::cout << "Server overwrite constructor." << std::endl;

    assignDefaultConfig(default_obj);
    assignNewConfig(object);

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
    _response = Response(request, *this);
    std::cout << "Request:" << std::endl << request << std::endl;
    return (0);
}

int	Server::send(void)
{
    std::string	str = _response.renderString();

    std::cout << "Webserv: send" << std::endl;
    std::cout << "Response:" << std::endl << str << std::endl;
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

/* CFG UTILS */
void Server::assignDefaultConfig(Object &object) {

    std::cout << "Assigning default to code." << std::endl;

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

}

void Server::assignNewConfig(Object & object) {

    std::cout << "Assigning object to code." << std::endl;

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
    if (object.getArray().find(std::string("locations")) != object.getArray().end()) {

        std::vector<Object>             new_loc;
        std::vector<Object>::iterator   it;

        new_loc = object.getArray()["locations"].getObject();

        // Iterate through every location block from the config file.
        for (it = new_loc.begin() ; it != new_loc.end() ; it++) {

            MapStr              new_content;
            MapStr::iterator    path;
            Locations::iterator def_it;

            new_content = it->getString();
            path = new_content.find(std::string("path"));

            if (path != new_content.end()) {

                // Iterate through every DEFAULT location block.
                for (def_it = _locations.begin(); def_it != _locations.end(); def_it++) {

                    MapStr::iterator    search_loc;

                    search_loc = def_it->find(std::string("path"));

                    if (search_loc == def_it->end())
                        break ;

                    // If both location block share the same path, overwrite them.
                    else if (search_loc->second == path->second) {
                        MapStr::iterator    search;
                        for (search = new_content.begin(); search != new_content.end(); search++) {
                            search_loc = def_it->find(std::string(search->first));
                            if (search_loc != def_it->end())
                                search_loc->second = search->second;
                        }
                    }
                }
            }
            else
                std::cerr << "cfg: location block needs a path: no creation." << std::endl;

        }
    }
}

/* ACCESSORS */
unsigned int	Server::getHost() const { return _host; }
int				Server::getListenFd() const { return _listen_fd; }
int				Server::getSocket() const { return _socket; }

std::vector<std::string> Server::getAddress() const { return _address; }
std::vector<std::string> Server::getDisabledMethods() const { return _disabled_methods; }

MapStr      Server::getErrorPages() const { return _error_pages; }
Locations   Server::getLocations() const { return _locations; }
std::string Server::getServerName() const { return _server_name; }
bool        Server::getAutoIndex() const { return _auto_index; }
int         Server::getClientBodySize() const { return _clt_body_size; }
int         Server::getPort() const { return _port; }
