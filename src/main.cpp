#include "../inc/webserv.hpp"
std::vector<std::string>	split(std::string string, char delimiter);
std::string trim(std::string string, char to_erase);
int main() {
	std::string test = "AfHHd AH H LOLHCa Marche  ;";
//	split(test,' ');
	test = trim(test, 's');
	std::cout << test;
    return 1;
}
