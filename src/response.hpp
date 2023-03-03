#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include "../inc/webserv.hpp"
#include "request.hpp"

class response{
public:
	response(request request);
	~response();
	
	void get_status(int code, std::string text);
	void get_headers();
	std::string send_response();
private:
	std::string 	_version;
	std::string		_status_code;
	std::string 	_status_text;
	std::map<std::string, std::string>	_headers;
	std::string 	_body;
	std::string 	_final_response;
};

#endif
