#include "../inc/webserv.hpp"

void    error(std::string msg) {
	std::string ret = "error ";
	ret.append(msg);
    perror(ret.c_str());
	exit(EXIT_FAILURE);
}
