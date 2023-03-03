#include "webserv.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Error: usage: " << argv[0] <<  "<config_file>" << std::endl;
		return (1);
	}
	Server	server;
	
}
