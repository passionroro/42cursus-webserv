#include "../inc/webserv.hpp"
#include "Server.hpp"
// server/client communication in c++ from scratch using select()

int g_exit;

void    signal_handler(int sig) {
    if (sig == SIGINT) {
        g_exit = 0;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    g_exit = 1;
    std::signal(SIGINT, signal_handler);

    Server  server;
    server.startListen();

    return (0);
}
