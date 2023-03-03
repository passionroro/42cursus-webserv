#include "webserv.hpp"
#include "WebServer.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Error: usage: " << argv[0] <<  " <config_file>" << std::endl;
		return (1);
	}

	WebServer	webserv(argv[1]);

	if (webserv.setup() < 0)
		return (1);
	webserv.run();
	return (0);
}
