#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../inc/webserv.hpp"

class Client {
public:
    Client();
    ~Client();

    void    clientConnect(int port);

private:
    int                 _socket_fd;
    struct sockaddr_in  _clt_addr;
};

#endif
