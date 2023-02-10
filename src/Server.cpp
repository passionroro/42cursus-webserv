#include "Server.hpp"

Server::Server() :  _socket_fd(), _conn_fd(), _select_fd(),  _serv_addr(), _serv_addr_len(sizeof(_serv_addr)){
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    _serv_addr.sin_port = htons(PORT);

    startServer();
    return ;
}

void Server::startServer() {
    if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket(): fatal.\n");

    //memset(&_serv_addr, '0', sizeof(_serv_addr));

    if (bind(_socket_fd, (struct sockaddr*)&_serv_addr, _serv_addr_len) < 0)
        error("bind(): fatal.\n");
}

void Server::receiveData() {
    char pkt[BUFFER_SIZE];
    if (recv(_conn_fd, pkt, BUFFER_SIZE, 0) < 0)
        error("recv(): error: failed to receive data");
	
}

void Server::acceptConnection() {
	if ((_conn_fd = accept(_socket_fd, (struct sockaddr*)&_serv_addr, &_serv_addr_len)) < 0)
        error("accept(): fatal\n");
}

void Server::startListen() {

    if (listen(_socket_fd, BACKLOG) < 0)
        error("listen(): fatal\n");

    while (true) {
        selectEvent();
        acceptConnection();
        receiveData();
        //sendData();
        close(_conn_fd);
    }
}

void Server::selectEvent() {
    struct timeval  tv;
    int             ret;

    FD_ZERO(&_select_fd);
    FD_SET(_socket_fd, &_select_fd);
    ret = select(_socket_fd + 1, &_select_fd, NULL, NULL, &tv);

    if (ret == 0)
        error("select(): timeout.\n");
    else if (ret == 1)
        error("select(): error.\n");
}

void Server::closeServer() {
    close(_socket_fd);
    close(_conn_fd);
    exit (EXIT_SUCCESS);
}

Server::~Server() {
    closeServer();
}
