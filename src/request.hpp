#include "../inc/webserv.hpp"

class request {
public:
	
	request();
	~request();
	
	int 	is_valid(std::string &request);
	bool	check_path(std::string s);
	int 	parse_headers(std::string &request);
private:
	std::string 						_method;
	std::string 						_path;
	std::string 						_version;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
};
