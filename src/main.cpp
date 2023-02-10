#include "../inc/webserv.hpp"
#include "Server.hpp"
// server/client communication in c++ from scratch using select()

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    Server  server;
    server.startListen();

    return (0);
}
