#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client {
public:
    Client();
    ~Client();

    void    clientConnect();

private:
    int                 _m_socket;
    struct sockaddr_in  _serv_addr;
};

#endif
