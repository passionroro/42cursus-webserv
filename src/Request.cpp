#include "Request.hpp"

Request::Request(void)
{
}

Request::Request(std::string request)
{
	is_valid(request);
}

Request::~Request() {}

int Request::is_valid(std::string &Request) {
	std::vector<std::string> r_line;
	std::string R_line;
	int spacenb = 0;
	R_line = Request.substr(0,Request.find('\r'));
	for (int i = 0; i < (int)R_line.size(); ++i) {
		if(isspace(R_line[i]))
			spacenb++;
	}
	if(spacenb != 2)
		return 400;
	size_t pos = 0;
	while ((pos = R_line.find(' ')) != std::string::npos) {
		r_line.push_back(R_line.substr(0, pos));
		R_line.erase(0, pos + 1);
		Request.erase(0, pos + 1);
	}
	pos = R_line.find('\r');
	r_line.push_back(R_line.substr(0, pos));
	Request.erase(0, R_line.size() + 1);
	_method = r_line.front();
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return 400;
	_path = r_line.at(1);
	if(!check_path(_path))
		return 300;
	_version = r_line.at(2);
	if (_version != "HTTP/1.1")
		return 400;
	if (parse_headers(Request)!= 0 )
		return 400;
	Request.erase(0, 1);
	_request_body = Request;
	_status = "200";
	return 200;
}

bool Request::check_path(std::string s) {
	std::fstream fs;
	std::string path = "home/www/index.html";
	(void)s;
	//path.append(s);
	fs.open(path);
	return fs.is_open();
}

int Request::parse_headers(std::string &Request) {
	std::string h_key;
	std::string h_value;
	size_t pos = 0;
	size_t pos_2 = 0;
	while ((pos = Request.find(':')) != std::string::npos) {
		h_key = (Request.substr(0, pos));
		Request.erase(0, pos + 1);
		pos_2 = Request.find('\n');
		h_value = (Request.substr(0, pos_2));
		if (h_value[0] == ' ')
			h_value.erase(0, 1);
		Request.erase(0,pos_2 + 1);
		if (h_key.find(' ') != std::string::npos)
			return 400;
		_request_headers.insert(std::pair<std::string, std::string>(h_key, h_value));
	}
	_status = "200";
	return 0;
}

std::string Request::getStatus() const {
	return _status;}

std::string Request::getPath() const {return _path;}
