#ifndef SERVER_HPP
# define SERVER_HPP

# include "../inc/webserv.hpp"

class Server {
public:
    Server();
    ~Server();

    void    startListen();

private:
    int                 _socket_fd;
    int                 _conn_fd;
    fd_set              _select_fd;
    struct sockaddr_in  _serv_addr;

    void    startServer();
    void    closeServer();
    void    acceptConnection();
    void    selectEvent();
    void    receiveData();
};

#endif