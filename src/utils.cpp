#include "../inc/webserv.hpp"

void    error(std::string msg) {
	std::string ret = "error ";
	ret.append(msg);
    perror(ret.c_str());
	exit(EXIT_FAILURE);
}

std::vector<std::string>	split(std::string string, char delimiter){
	std::vector<std::string>	ret;
	size_t 						position;
	while(string[0] == delimiter)
		string.erase(0,1);
	while ((position = string.find(delimiter)) != std::string::npos) {
			ret.push_back(string.substr(0, position));
		string.erase(0, position + 1);
		while(string[0] == delimiter)
			string.erase(0,1);
	}
	if (!string.empty())
		ret.push_back(string.substr(0, position));
	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end();it++) {
		std::cout << *it << std::endl;
	}
	return ret;
}


std::string trim(std::string string, char to_erase){
	size_t position;
	while ((position = string.find(to_erase)) != std::string::npos){
		string.erase(position, 1);}
	return string;
}
