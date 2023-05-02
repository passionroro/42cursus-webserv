#include "webserv.hpp"
#include "../inc/Config.hpp"
#include "WebServer.hpp"

int	main(int argc, char **argv)
{
	(void) argc;
	(void)argv;
	std::string test = "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive";//	if (argc != 2) {
//		std::cerr << "Error: usage: " << argv[0] <<  " <config_file>" << std::endl;
//		return (1);
//	}
//
//	WebServer	webserv(argv[1]);
//
//	if (webserv.setup() < 0)
//		return (1);
//
//    Config config;
//    Object server = config.setup(argv[1]);
//
//	webserv.run();
	Request Test(test);
	return (0);
}
