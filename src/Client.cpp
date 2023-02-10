#include "Client.hpp"

Client::Client() : _m_socket(), _serv_addr() {}

void Client::clientConnect() {
    if ((_m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket(): fatal");

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(PORT);
    _serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if (connect(_m_socket, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
        error("connect(): fatal");
}
