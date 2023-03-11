#include "../inc/webserv.hpp"
#include "../inc/Config.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: usage: " << argv[0] << " [config file]." << std::endl;
        return (1);
    }

    Config config;
    Object server = config.setup(argv[1]);

    return (0);
}
