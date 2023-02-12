#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/*--- DEFINES ---*/
# define LOCALHOST "127.0.0.1"
# define BUFFER_SIZE 4096

/*--- INCLUDES AND LIBRARIES ---*/
#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <random>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

/*--- FUNCTIONS ---*/
void    error(std::string msg);



extern bool g_exit;

#endif
