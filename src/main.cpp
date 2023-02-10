#include "../inc/webserv.hpp"
// server/client communication in c++ from scratch using select()

void    error(std::string msg) {
    perror(msg.c_str());
    exit (EXIT_FAILURE);
}

/*
    socket(): create a socket - Get the file descriptor!
    bind(): bind to an address - What port am I on?
    listen(): listen on a port, and wait for a connection to be established.
    accept(): accept the connection from a client.
    recv() - the same way we read from a file.
    shutdown to end read.
    close to releases data.
 */
void    run_server(void) {
    return ;
}

void    run_client(void) {
    return ;
}

void    run_webserv(void) {
    pid_t   pid;

    if ((pid = fork()) < 0)
        error("fork(): fatal.");

    if (pid == 0)
        run_client();
    else {
        run_server();
        exit (EXIT_SUCCESS);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    run_webserv();

    return (0);
}
