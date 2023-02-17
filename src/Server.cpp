#include "Server.hpp"

Server::Server() : _socket_fd(), _serv_addr() { }

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

#include <fstream>
void    open_send_file(int client) {
    std::ifstream       ifs;
    ifs.open("home/www/index.html", std::ifstream::in);
    if (!ifs.is_open())
        error("open()");

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    str.insert(0, "HTTP/1.1 200 OK\\r\\n\\r\\n");

    std::cout << str.c_str() << std::endl;

    send(client, str.c_str(), str.length(), 0);

    std::cout << "sent!" << std::endl;

    ifs.close();
}


void Server::serverRun() {

    std::cout << "Waiting for connection..." << std::endl;
    _serv_addr_len = sizeof(_serv_addr);
    int client = accept(_socket_fd, (struct sockaddr*)&_serv_addr, &_serv_addr_len);

    std::cout << "Connection received" << std::endl;
    if (client == -1)
        error("accept(): client fatal");

    std::cout << "Client connected on " << inet_ntoa(_serv_addr.sin_addr) << ":" << _serv_addr.sin_port << std::endl;

    open_send_file(client);

    char buf[BUFFER_SIZE];
    //while (g_exit) {
        memset(buf, 0, BUFFER_SIZE);

        int ret = recv(client, buf, BUFFER_SIZE, 0);
        if (ret == -1)
            error("Connection issue");
        if (ret == 0) {
            std::cout << "Client disconnected." << std::endl;
            //break ;
            return ;
        }

        std::cout << "Received: " << std::string(buf, 0, ret) << std::endl;
        std::cout << "--- end of request" << std::endl;
    //}


    close(client);
    close(_socket_fd);
}


Server::~Server() { }
