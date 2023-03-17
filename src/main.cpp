#include "webserv.hpp"
#include "../inc/Config.hpp"
#include "WebServer.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2) {
		std::cerr << "Error: usage: " << argv[0] <<  " <config_file>" << std::endl;
		return (1);
	}

    Config config;
    config.setup(argc == 1 ? "config/default.json" : argv[1]);

	WebServer	webserv(config);

	if (webserv.setup() < 0)
		return (1);

	webserv.run();
	return (0);
}
