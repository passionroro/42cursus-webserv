#include "../inc/webserv.hpp"
// server in c++ from scratch using select()

void    run_server(void) {
    struct sockaddr_in  serv_addr;
    struct timeval      tv;
    fd_set              fd;
    int                 listen_fd;
    int                 conn_fd;
    int                 ret;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (listen(listenfd, 1) < 0) {
        perror("listen(): fatal.");
        exit (EXIT_FAILURE);
    }

    while (true) {
        FD_ZERO(&fd);
        FD_SET(listen_fd, &fd);
        ret = select(listen_fd + 1, &fd, NULL, NULL, &tv);

        if (ret == 0) {
            perror("select(): timeout.");
            exit (EXIT_FAILURE);
        }
        else if (ret == 1) {
            perror("select(): error.");
            exit (EXIT_FAILURE);
        }

        conn_fd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        /*Packet Data Type*/int pkt;
        if (recv(listen_fd, &pkt, sizeof(/*Packet Data Type*/int), 0) < 0) {
            perror("recv(): error: failed to receive data");
            exit (EXIT_FAILURE);
        }

        std::cout << "Received data !" << std::endl;

        close(conn_fd);
        //break ;
    }
}

int    run_webserv(void) {
    pid_t   pid;

    if ((pid == fork()) < 0) {
        perror("fork(): fatal.");
        exit (EXIT_FAILURE);
    }
    if (pid == 0)
        run_server();
    else
        run_client();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    if (run_webserv())
        return (1);

    return (0);
}
