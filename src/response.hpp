#include "../inc/webserv.hpp"

class response{
public:
	response(int code);
	~response();
	
	void get_status(int code, std::string text);
	void get_headers();
	std::string send_response();
private:
	std::string 	_version;
	int				_status_code;
	std::string 	_status_text;
	std::string 	_headers;
	
};
