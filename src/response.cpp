#include "../inc/webserv.hpp"
#include "response.hpp"
response::response(int code) : _version("HTTP/1.1 "){
	this->_status_code = code;
	if (code == 200)
		_status_text = "OK";
	get_headers();
	return;
}

response::~response() { return;}

void response::get_status(int code, std::string text) {
	this->_status_code = code;
	this->_status_text = text;
}

void response::get_headers() {
	int size;
	char buf[100];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	tm.tm_zone = (char *)"GMT";
	size = strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z\n", &tm);
	_headers.append(buf, size);
}

std::string response::send_response() {
	std::string code = std::to_string(_status_code);
	return _version + code + " " + _status_text  + "\r\n" + _headers;
}
