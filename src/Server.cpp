#include "Server.hpp"

Server::Server() : _socket_fd(), _fd(), _serv_addr() { }

void Server::serverInit(int port) {
    if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket(): fatal");

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(port);
    _serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (bind(_socket_fd, (sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0)
        error("bind(): fatal");

    if (listen(_socket_fd, 10) < 0)
        error("listen(): fatal");

    std::cout << "Server listening on " << LOCALHOST << ":" << port << std::endl;
}

void Server::serverRun() {
    while (g_exit) {
        sleep(1);
        FD_ZERO(&_fd);
        FD_SET(_socket_fd, &_fd);
        if (select(_socket_fd + 1, &_fd, NULL, NULL, NULL) < 0)
            error("select(): fatal");

        std::cout << "Waiting for connection..." << std::endl;
        _serv_addr_len = sizeof(_serv_addr);
        int client = accept(_socket_fd, (struct sockaddr*)&_serv_addr, &_serv_addr_len);

        char pkt[BUFFER_SIZE] = {0};
        if (recv(client, &pkt, BUFFER_SIZE, 0) < 0)
            error("recv(): error: failed to receive data");

        close(client);
    }
}

void Server::broadcastToClients(int sendingClient, const char *msg, int length) {
    for (int i = 0; i < 2; i++) {
        int outSock = _fd.fds_bits[i];
        if (outSock != _socket_fd && outSock != sendingClient) {
            send(outSock, msg, length, 0);
        }
    }
}

void Server::onClientConnected(int clientSocket) {
    std::cout << "on [" << clientSocket << "]: client connected" << std::endl;
}

void Server::onClientDisconnected(int clientSocket) {
    std::cout << "on [" << clientSocket << "]: client disconnected" << std::endl;
}

void Server::onMessageReceived(int clientSocket, const char *msg) {
    std::cout << "on [" << clientSocket << "]: msg received: " << std::endl;
    std::cout << msg << std::endl;
}

Server::~Server() {}
