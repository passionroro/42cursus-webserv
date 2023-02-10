#include "../inc/webserv.hpp"
// server in c++ from scratch using select()

/*
    socket(): create a socket - Get the file descriptor!
    bind(): bind to an address - What port am I on?
    listen(): listen on a port, and wait for a connection to be established.
    accept(): accept the connection from a client.
    recv() - the same way we read from a file.
    shutdown to end read.
    close to releases data.
 */
void    error(std::string msg) {
    perror(msg.c_str());
    exit (EXIT_FAILURE);
}

void    run_server(void) {
    struct sockaddr_in  serv_addr;
    struct timeval      tv;
    fd_set              fd;
    int                 socket_fd;
    int                 accept_fd;
    int                 ret;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM /*TCP Stream*/, 0)) < 0)
        error("socket(): fatal.\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    serv_addr.sin_port = htons(PORT);

    bind(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (listen(socket_fd, 1) < 0)
        error("listen(): fatal.\n");

    while (true) {
        FD_ZERO(&fd);
        FD_SET(socket_fd, &fd);
        ret = select(socket_fd + 1, &fd, NULL, NULL, &tv);

        if (ret == 0)
            error("select(): timeout.");
        else if (ret == 1)
            error("select(): error.");

        accept_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL);

        char pkt[1024];
        if (recv(accept_fd, &pkt, 1024, 0) < 0)
            error("recv(): error: failed to receive data");

        std::cout << "Received data !" << std::endl << pkt << std::endl;

        close(accept_fd);
    }
}

void    run_client(void) {
    return ;
}

void    run_webserv(void) {
    pid_t   pid;

    if ((pid = fork()) < 0) {
        perror("fork(): fatal.");
        exit (EXIT_FAILURE);
    }
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
