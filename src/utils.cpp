#include "../inc/webserv.hpp"

void    error(std::string msg) {
    perror("error: " + msg.c_str());
    EXIT_FAILURE;
}