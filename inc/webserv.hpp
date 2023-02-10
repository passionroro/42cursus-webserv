#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*--- DEFINES ---*/
# define PORT 1312
# define LOCALHOST "127.0.0.1"
# define BUFFER_SIZE 4096

/*--- HEADERS ---*/
# include "../src/Server.hpp"
# include "../src/Client.hpp"

/*--- INCLUDES AND LIBRARIES ---*/
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <poll.h>
# include <netdb.h>

/*--- FUNCTIONS ---*/
void    error(std::string msg);

extern bool g_exit;

#endif
