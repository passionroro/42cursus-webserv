#include "Request.hpp"
#include "Server.hpp"
#include <dirent.h>

/* CONSTRUCTORS */
Request::Request(void)
{
}

Request::Request(std::string request, Server& server_conf)
{
	_isDir = false;
	_locations = server_conf.getLocations();
  
  parseRequest(request);

	//is_valid(request);.
}

Request::~Request() {}

/* MEMBER FUNCTIONS */

int Request::parseRequest(std::string &Request) {
	
	std::vector<std::string> firstLine;
	std::string R_line;
	int spaceCount = 0;
	setStatus("200");
	
	firstLine = split(Request.substr(0,Request.find('\r')), ' ');
	if (firstLine.size() != 3) {
		setStatus("400");
		return 0;
	}
	
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
	_requestPath = _path;
    if (_path.find('?') != std::string::npos){
        _path = _path.substr(0, _path.find('?'));
    }
	checkPath();
	
	_version = firstLine.at(2);
	if (_version != "HTTP/1.1")
		setStatus("400");
//	std::cout << "method: " << _method <<std::endl;
//    std::cout << "pt: " << _path <<std::endl;
//    std::cout << "version: " << _version <<std::endl;
//
//    std::cout << _status<<std::endl;
	
	return 0;
}

int Request::parseHeaders(std::string &Request) {

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
      setStatus("400");
    _request_headers.insert(std::pair<std::string, std::string>(h_key, h_value));
	}
	_status = "200";
	return 0;
}

void Request::checkMethod() {
	std::vector<std::string> NotImplemented = split("CONNECT HEAD OPTIONS PUT TRACE PATCH",' ');
	
	for (std::vector<std::string>::iterator it = NotImplemented.begin();it != NotImplemented.end(); it++) {
		if (_method == *it)
			setStatus("501");
	}
//	for (std::vector<std::string>::iterator it = _disabled_methods.begin();it != _disabled_methods.end(); it++) {
//		if (_method == *it)
//			setStatus("405");
//	}
	if (_method != "GET" && _method != "POST" && _method != "DELETE" && _status != "405")
		setStatus("400");
}

void Request::checkPath()
{
	std::fstream fs;
	std::vector<std::string> location;
    Locations::iterator it;
	
    for (it = _locations.begin();it != _locations.end(); it++) {
        if (_path == it->at("path"))
            break;
    }
	
	if (it != _locations.end()) {
        _path.append(it->at("index"));
        _path.insert(0,it->at("root"));
        fs.open(_path);
        if (fs.is_open())
		{
			_locIndex = it;
			std::cout << "Valid path is : " << _path << std::endl;
            return;
		}
        else
            setStatus("404");
    }
	
	else
	{
		DIR*	dir = NULL;
    	for (it = _locations.begin() ;it != _locations.end(); it++)
		{
			location.push_back((*it)["root"].append(_path));
			
			fs.open(location.back());
			if (fs.is_open() || (dir = opendir(location.back().c_str())) != NULL) {
				if (dir)
				{
					_isDir = true;
					closedir(dir);
				}
				_path = location.back();
				std::cout << "Valid path is : " << _path << std::endl;
				_locIndex = it;
				break;
			}
		}
		if (!fs.is_open() && _isDir == false)
			setStatus("404");
	}
	std::cout << "404 not found: " << _path << std::endl;
//	ADD REDIRECTIONS
//  CLOSE PATH WHEN ??
}

/* SETTERS */
void Request::setStatus(std::string statusCode) {
    this->_status = statusCode;
}

/* ACCESSORS */
std::string Request::getStatus() const { return _status; }
std::string Request::getRequestPath() const { return _requestPath; }

/* PRIVATE CONSTRUCTOR */
