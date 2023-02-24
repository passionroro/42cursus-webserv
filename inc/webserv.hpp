#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*--- DEFINES ---*/
# define PORT 8080
# define LOCALHOST "127.0.0.1"
# define BACKLOG 5
# define BUFFER_SIZE 1024

/*--- INCLUDES AND LIBRARIES ---*/
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <poll.h>
# include <netdb.h>
# include <fstream>
# include <vector>
# include <algorithm>

/*--- FUNCTIONS ---*/
// utils.cpp
void    error(std::string msg);


#endif
