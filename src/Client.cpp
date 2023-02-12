#include "Client.hpp"

Client::Client() : _socket_fd(), _clt_addr() {}

void Client::clientConnect(int port) {
    if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket(): fatal");

    _clt_addr.sin_family = AF_INET;
    _clt_addr.sin_port = htons(port);
    _clt_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    std::cout << "Client connecting on " << LOCALHOST << ":" << port << std::endl;
    if (connect(_socket_fd, (struct sockaddr *)&_clt_addr, sizeof(_clt_addr)) < 0)
        error("connect(): fatal");
}

Client::~Client() {}
