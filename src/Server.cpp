#include "Server.hpp"

Server::Server() : _m_socket(), _m_master() {}

void Server::serverInit() {
    if ((_m_socket = int(AF_INET, SOCK_STREAM, 0)) < 0)
        error("int(): fatal");

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(PORT);
    _serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (bind(_m_socket, (sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0)
        error("bind(): fatal");

    if (listen(_m_socket, 10) < 0)
        error("listen(): fatal");

    FD_ZERO(&_m_master);
    FD_SET(_m_socket, &_m_master);
}

void Server::serverRun() {
    while (g_exit)
    {
        fd_set  copy = _m_master;
        int     socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i = 0; i < socketCount; i++) {
            int socket = copy.fd_array[i];

            if (socket == _m_socket) {
                int client = accept(_m_socket, nullptr, nullptr);

                FD_SET(client, &_m_master);
                onClientConnected(client);
            }
            else {
                char buf[BUFFER_SIZE] = {0};

                if (recv(socket, buf, BUFFER_SIZE, 0) <= 0){
                    onClientDisconnected(socket);
                    close(socket);
                    FD_CLR(socket, &_m_master);
                }
                else {
                    onMessageReceived(socket, buf, bytesIn);
                }
            }
        }
    }

    FD_CLR(_m_socket, &_m_master);
    close(_m_socket);

    while (_m_master.fd_count > 0)
    {
        FD_CLR(_m_master.fd_array[0], &_m_master);
        close(_m_master.fd_array[0]);
    }
}

void Server::broadcastToClients(int sendingClient, const char *msg, int length) {
    for (int i = 0; i < _m_master.fd_count; i++) {
        int outSock = _m_master.fd_array[i];
        if (outSock != m_socket && outSock != sendingClient) {
            send(outSock, msg, length, 0);
        }
    }
}

void Server::onClientConnected(int clientSocket) {
    std::cout << "on [" << clientSocket << "]: client connected" << std::endl;
}

void Server::onClientDisonnected(int clientSocket) {
    std::cout << "on [" << clientSocket << "]: client disconnected" << std::endl;
}

void Server::onMessageReceived(int clientSocket, const char *msg, int length) {
    std::cout << "on [" << clientSocket << "]: msg of len " << length << "received: " << std::endl;
    std::cout << msg << std::endl;
}