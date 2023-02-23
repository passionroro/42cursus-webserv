#include "../inc/webserv.hpp"

class response{
public:
	response();
	~response();
	
	void get_status(std::string code, std::string text);
	void get_headers();
	std::string send_response();
private:
	std::string 	_version;
	std::string		_status_code;
	std::string 	_status_text;
	std::string 	_headers;
	
};
