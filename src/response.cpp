#include "../inc/webserv.hpp"

response::response() : _version("HTTP/1.1 "){
	return;
}

response::~response() { return;}

void response::get_status(std::string code, std::string text) {
	this->_status_code = code;
	this->_status_text = text;
}

void response::get_headers() {
	int size;
	char buf[100];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	tm.tm_zone = (char *)"EDD";
	size = strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z\n", &tm);
	_headers.append(buf, size);
}

std::string response::send_response() {
	return _version + _status_code+ " " + _status_text  + "\r\n" + _headers;
}
