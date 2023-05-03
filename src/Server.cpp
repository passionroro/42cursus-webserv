#include "Server.hpp"

/* CONSTRUCTORS */
// Server constructor when no config file is given
Server::Server(Object & default_obj) {

    std::cout << "Server default constructor." << std::endl;
    assignConfig(default_obj);

}

// Server constructor if a config file is given
Server::Server(Object & default_obj, Object & object) {

    std::cout << "Server overwrite constructor." << std::endl;

    assignConfig(default_obj);
    assignConfig(object);

}


// Assign every attribute from the Object "server" (config) into the class Server
// As we use the `operator[]` to access the elements, we need to check if they exist using `find`
void Server::assignConfig(Object & object) {

    std::cout << "Assigning object to code." << std::endl;

    if (object.getString().find(std::string("server_name")) != object.getString().end())
        _server_name = object.getString()["server_name"];

    if (object.getInt().find(std::string("port")) != object.getInt().end())
        _port = object.getInt()["port"];
    if (object.getInt().find(std::string("client_max_body_size")) != object.getInt().end())
        _clt_body_size = object.getInt()["client_max_body_size"];


    if (object.getArray().find(std::string("address")) != object.getArray().end())
        _address = object.getArray()["address"].getString();
    if (object.getArray().find(std::string("disabled_methods")) != object.getArray().end())
        _disabled_methods = object.getArray()["disabled_methods"].getString();

    if (object.getObject().find(std::string("error_pages")) != object.getObject().end())
        _error_pages = object.getObject()["error_pages"].getString();

    if (object.getArray().find(std::string("locations")) != object.getArray().end()) {

        std::vector<Object>             tmp_location;
        std::vector<Object>::iterator   it;

        tmp_location = object.getArray()["locations"].getObject();
        for (it = tmp_location.begin() ; it != tmp_location.end() ; it++) {
            _locations.push_back(it->getString());
        }

    }

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

/* ACCESSORS */
unsigned int	Server::getHost(void) const { return _host; }
short			Server::getPort(void) const { return _port; }
int				Server::getListenFd(void) const { return _listen_fd; }
int				Server::getSocket(void) const { return _socket; }
