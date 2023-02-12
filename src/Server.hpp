#ifndef SERVER_HPP
# define SERVER_HPP

# include "../inc/webserv.hpp"

class Server {
public:
    Server();
    ~Server();

    void    serverInit(int port);
    void    serverRun();

protected:
    virtual void    onClientConnected(int clientSocket);
    virtual void    onClientDisconnected(int clientSocket);
    virtual void    onMessageReceived(int clientSocket, const char* msg);
    void            sendToClient(int clientSocket, const char* msg, int length);
    void            broadcastToClients(int sendingClient, const char* msg, int length);

private:
    int                 _socket_fd;
    fd_set              _fd;
    struct sockaddr_in  _serv_addr;
    socklen_t           _serv_addr_len;
};

#endif
