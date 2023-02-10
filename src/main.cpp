#include "../inc/webserv.hpp"
// server in c++ from scratch using select()

bool g_exit;

void    run_webserv(void) {
    pid_t   pid;

    if ((pid = fork()) < 0)
        error("fork(): fatal");
    if (pid == 0) {
        Client client;
        client.clientConnect();
    }
    else {
        Server  server;
        server.serverInit();
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    g_exit = true;
    run_webserv();

    return (0);
}
