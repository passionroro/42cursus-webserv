#include "request.hpp"

request::request() {}

request::~request() {}

int request::is_valid(std::string &request) {
	std::vector<std::string> r_line;
	std::string R_line;
	R_line = request.substr(0,request.find('\r'));
	size_t pos = 0;
	while ((pos = R_line.find(' ')) != std::string::npos) {
		r_line.push_back(R_line.substr(0, pos));
		R_line.erase(0, pos + 1);
		request.erase(0, pos + 1);
	}
	pos = R_line.find('\r');
	r_line.push_back(R_line.substr(0, pos));
	request.erase(0, R_line.size() + 1);
	_method = r_line.front();
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return 400;
	_path = r_line.at(1);
	if(!check_path(_path))
		return 300;
	_version = r_line.at(2);
	if (_version != "HTTP/1.1")
		return 400;
//	std::cout << _method << std::endl << _path << std::endl << _version;
	parse_headers(request);
	request.erase(0, 1);
	_body = request;
	return 0;
}

bool request::check_path(std::string s) {
	std::fstream fs;
	std::string path = "home/www/";
	path.append(s);
	fs.open(path);
	return fs.is_open();
}

int request::parse_headers(std::string &request) {
	std::string h_key;
	std::string h_value;
	size_t pos = 0;
	size_t pos_2 = 0;
	while ((pos = request.find(':')) != std::string::npos) {
		h_key = (request.substr(0, pos));
		request.erase(0, pos + 1);
		pos_2 = request.find('\n');
		h_value = (request.substr(0, pos_2));
		request.erase(0, pos_2 + 1);
		if ((pos = h_key.find(' ')) != std::string::npos)
			h_key.erase(pos);
		if ((pos = h_value.find(' ')) != std::string::npos)
			h_value.erase(pos, 1);
		_headers.insert(std::pair<std::string, std::string>(h_key, h_value));
	}
	std::cout << _headers.at("Host");
	return 0;
}

