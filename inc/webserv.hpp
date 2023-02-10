#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*--- DEFINES ---*/
# define PORT 8080
# define LOCALHOST "127.0.0.1"
# define BACKLOG 5

/*--- INCLUDES AND LIBRARIES ---*/
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/types.h>
# include <signal.h>
# include <arpa/inet.h>
# include <poll.h>
# include <csignal>
# include <netdb.h>

/*--- FUNCTIONS ---*/
// utils.cpp
void    error(std::string msg);
void    signal_handler(void);

extern int  g_exit;

#endif
