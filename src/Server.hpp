#ifndef SERVER_HPP
# define SERVER_HPP

# include "../inc/webserv.hpp"

class Server {
public:
    Server();
    ~Server();

    void    serverInit();
    void    serverRun();

protected:
    virtual void    onClientConnected(int clientSocket);
    virtual void    onClientDisconnected(int clientSocket);
    virtual void    onMessageReceived(int clientSocket, const char* msg, int length);
    void            sendToClient(int clientSocket, const char* msg, int length);
    void            broadcastToClients(int sendingClient, const char* msg, int length);

private:
    int                 _m_socket;
    fd_set              _m_master;
    struct sockaddr_in  _serv_addr;
};

#endif
