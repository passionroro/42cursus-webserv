#ifndef SERVER_HPP
# define SERVER_HPP

# include "../inc/webserv.hpp"

class Server {
public:
    Server();
    ~Server();

    void    serverInit(int port);
    void    serverRun();

private:
    int                 _socket_fd;
    struct sockaddr_in  _serv_addr;
    socklen_t           _serv_addr_len;
};

#endif
