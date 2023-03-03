#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>

# include <string>
# include <iostream>

# include <vector>
# include <map>
# include <list>

# define SERVER_BACKLOG 1000
# define BUFSIZE 4096

class	Server;
class	Config;
class	Request;
class	Response;

#endif
