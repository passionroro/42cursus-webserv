#include "Request.hpp"

Request::Request(void)
{
}

Request::Request(std::string request)
{
	parseRequest(request);
}

Request::~Request() {}

int Request::parseRequest(std::string &Request) {
	
	std::vector<std::string> firstLine;
	std::string R_line;
	int spaceCount = 0;
	setStatus("200");
	
	firstLine = split(Request.substr(0,Request.find('\r')), ' ');
	
	R_line = Request.substr(0,Request.find('\r'));
	
	for (int i = 0; i < (int)R_line.size(); ++i) {
		if(isspace(R_line[i]))
			spaceCount++;
	}
	if(spaceCount != 2)
		setStatus("400");
	
	_method = firstLine.front();
	checkMethod();
	
	_path = firstLine.at(1);
	checkPath();
	
	_version = firstLine.at(2);
	if (_version != "HTTP/1.1")
		setStatus("400");
	
	std::cout << _status;
	
	return 0;
	/*
	size_t pos = 0;
	
	while ((pos = R_line.find(' ')) != std::string::npos) {
		firstLine.push_back(R_line.substr(0, pos));
		R_line.erase(0, pos + 1);
		Request.erase(0, pos + 1);
	}
	
	pos = R_line.find('\r');
	
	firstLine.push_back(R_line.substr(0, pos));
	Request.erase(0, R_line.size() + 1);
	
	_method = firstLine.front();
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return 400;
	_path = firstLine.at(1);
	if(!check_path(_path))
		return 300;
	_version = firstLine.at(2);
	if (_version != "HTTP/1.1")
		return 400;
	if (parse_headers(Request)!= 0 )
		return 400;
	Request.erase(0, 1);
	_request_body = Request;
	_status = "200";
	return 200;*/
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

void		Request::setStatus(std::string statusCode)
{
	this->_status = statusCode;
}

void Request::checkMethod() {
	std::vector<std::string> NotImplemented = split("CONNECT HEAD OPTIONS PUT TRACE PATCH",' ');
	
	for (std::vector<std::string>::iterator it = NotImplemented.begin();it != NotImplemented.end(); it++) {
		if (_method == *it)
			setStatus("501");
	}
//	for (std::vector<std::string>::iterator it = _diabled_methods.begin();it != _diabled_methods.end(); it++) {
//		if (_method == *it)
//			setStatus("405");
//	}
}

void Request::checkPath() {
	std::fstream fs;
	std::vector<std::string> paths;
	/*//TRY CATCH
	for (std::vector< std::map<std::string, std::string> >::iterator it = _locations.begin();it != _locations.end(); it++) {
		paths.push_back(*it->at("root").append(_path));
	}*/
	for (std::vector<std::string>::iterator it = paths.begin();it != paths.end(); it++) {
		fs.open(*it);
		if (fs.is_open())
			break;
	}
	if(!fs.is_open())
		setStatus("404");
	//ADD REDIRECTIONS
}
