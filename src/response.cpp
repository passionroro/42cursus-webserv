#include "../inc/webserv.hpp"
#include "response.hpp"
response::response(request request) : _version("HTTP/1.1 "){
	this->_status_code = request.get_status();
	if (_status_code == "200")
	_status_text = "OK";
	_body = request.get_path();
	
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
	_headers.insert(std::pair<std::string, std::string>("Date: ", buf));
}

std::string response::send_response() {
	std::fstream fs;
	std::string path = "home/www/";
	path.append(_body);
	fs.open(path);
	fs >> path;
	std::map<std::string,std::string>::iterator it = _headers.begin();
	_final_response = _version + _status_code + " " + _status_text  + "\r\n";
	_final_response.append(it->first + it->second + "\r\n");
	while (++it != _headers.end())
	{
		_final_response.append(it->first + it->second + "\r\n");
	}
	_final_response.append(path);
	_final_response.append("\r\n");
	return (_final_response);
}
