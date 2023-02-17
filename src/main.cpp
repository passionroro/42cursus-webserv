#include "Server.hpp"
#include "Client.hpp"

bool g_exit;

void    run_webserv(void) {
  //  pid_t           pid;

    srand(time(0));
    int port = rand() % 10 + 8080;

    /*if ((pid = fork()) < 0)
        error("fork(): fatal");
    if (pid == 0) {
        Client client;
        client.clientConnect(port);
    }
    else {*/
        Server  server;
        server.serverInit(port);
        server.serverRun();
  //  }
}

void    signal_handler(int sig) {
    if (sig == SIGINT)
        g_exit = false;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    g_exit = true;
    signal(SIGINT, signal_handler);
    run_webserv();

    return (0);
}
