#include "webserv.hpp"
#include "config/Config.hpp"
#include "WebServer.hpp"

int	main(int argc, char **argv)
{

	if (argc > 2) {
		std::cerr << "Error: usage: " << argv[0] <<  " <config_file>" << std::endl;

		return (1);
	}

    Config default_cfg;
    default_cfg.setup("config/default.json");

    Config config;
    if (argc > 1) {
        config.overwrite = true;
        config.setup(argv[1]);
    }

	WebServer	webserv(default_cfg, config);

	if (webserv.setup() < 0)
		return (1);

	webserv.run();
	return (0);
}

