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
	_disabledMethods = server_conf.getDisabledMethods();
	
	parseRequest(request, server_conf);
}

Request::~Request() {}

/* MEMBER FUNCTIONS */

int Request::parseRequest(std::string &request, Server& conf) {
	
	std::vector<std::string>	firstLine;
	std::string					R_line;
	int 						spaceCount = 0;
	
	setStatus("200");
	
	firstLine = split(request.substr(0,request.find('\r')), ' ');
	
	if (firstLine.size() != 3)
	{
		setStatus("400");
		return 0;
	}
	
	R_line = request.substr(0, request.find('\r'));
  
	for (int i = 0; i < (int)R_line.size(); ++i)
	{
		if(isspace(R_line[i]))
			spaceCount++;
	}
	
	if(spaceCount != 2)
		setStatus("400");
	
	_method = firstLine.front();
	checkMethod();
	
	_path = firstLine.at(1);
	_requestPath = _path;
	
    if (_path.find('?') != std::string::npos)
        _path = _path.substr(0, _path.find('?'));
	
	checkRedirection(conf);
	
	if (_status[0] != '3')
		checkPath();
	
	_version = firstLine.at(2);
	
	if (_version != "HTTP/1.1")
		setStatus("400");
	
	request.erase(0, request.find_first_of('\n') + 1);
	parseHeaders(request);
	checkHost(conf);
	parseBody(request);
	//std::cout << "Body is "<<_requestBody<< std::endl;
	return 0;
}

void	Request::checkRedirection(Server& conf)
{
	Redirection const& redirection = conf.getRedirection();
	//std::cout << "path: " << _path << std::endl;

    Redirection::const_iterator it;
	
    
	for (it = redirection.begin() ; it != redirection.end(); it++)
	{
		try
		{
			it->at("old_url");
			it->at("new_url");
			it->at("type");
		}
		catch (std::exception& e)
		{
			std::cerr << "Redirection badly defined" << std::endl;
		}
        if (_path == it->at("old_url") && it->at("type") == "permanent")
		{
			std::cout << "Redirection!!!" << std::endl;
			_newURL = it->at("new_url");
			_status = "301";
		}
        if (_path == it->at("old_url") && it->at("type") == "tmp")
		{
			std::cout << "Redirection!!!" << std::endl;
			_newURL = it->at("new_url");
			_status = "302";
		}
    }
}

int Request::parseHeaders(std::string &request) {

	std::string	headerKey;
	std::string	headerValue;
	size_t		pos = 0;
	size_t		pos_2 = 0;
	
	while ((pos = request.find(':')) != std::string::npos)
	{
		headerKey = (request.substr(0, pos));
		request.erase(0, pos + 1);
		pos_2 = request.find('\r');
		headerValue= (request.substr(0, pos_2));
		
		if (headerValue[0] == ' ')
			headerValue.erase(0, 1);
		
		request.erase(0, pos_2 + 2);
		
		if (headerKey.find(' ') != std::string::npos)
      		setStatus("400");
		
    	_requestHeaders.insert(std::pair<std::string, std::string>(headerKey, headerValue));
	}
	//printHeaders();
	return 0;
}

void	Request::printHeaders(void)
{
	std::cout << "request headers:" << std::endl;
	for (MapStr::iterator it = _requestHeaders.begin() ; it != _requestHeaders.end() ; it++)
		std::cout << '[' << it->first << ']' << ", " << it->second << std::endl;
}

void	Request::checkHost(Server& conf)
{
	std::string					host;
	size_t						sep;
	std::vector<std::string>&	addr = conf.getAddress();
	
	//printHeaders();
	//MapStr::iterator it = _request_headers.find("Host");
	//std::cout << "checkHost: " << it->first << ", " << it->second << std::endl;
	if (_requestHeaders.find("Host") == _requestHeaders.end())
	{
		_status = "400";
		return ;
	}
	
	host = _requestHeaders.at("Host");

	if ((sep = host.find_first_of(':')) != host.npos)
		host = host.substr(0, sep);
	//std::cout << "parsed host: " << host << std::endl;


	if (host == conf.getServerName())
		return ;

	for (unsigned long i = 0 ; i < addr.size() ; i++)
	{
		if (host == addr[i])
			return ;
	}
	std::cout << "Bad host, bad request" << std::endl;
	_status = "400";
}

void Request::parseBody(std::string &request)
{
	int i;
	
	request.erase(0,2);
	
	if (_requestHeaders["Transfer-Encoding"] == "chunked")
	{
		while (!request.empty())
		{
			std::istringstream iss(request.substr(0,'\r'));
			iss >> std::hex >> i;
			if (i == 0)
				break;
			_requestBody += request.substr(request.find('\n') + 1,i);
			std::cout << "is :"<< _requestBody << std::endl;
			request.erase(0,request.find('\n') + 1);
			request.erase(0,i + 2);
		}
	}
	else
		_requestBody = request;
}

void Request::checkMethod()
{
	std::vector<std::string> NotImplemented = split("CONNECT HEAD OPTIONS PUT TRACE PATCH",' ');
	
	for (std::vector<std::string>::iterator it = NotImplemented.begin();it != NotImplemented.end(); it++) {
		if (_method == *it)
		{
			setStatus("501");
			return;
		}
	}
	for (std::vector<std::string>::iterator it = _disabledMethods.begin();it != _disabledMethods.end(); it++) {
		if (_method == *it)
		{
			setStatus("405");
			return;
		}
	}
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		setStatus("400");
}

void Request::checkPath()
{
	std::fstream 				fs;
	std::vector<std::string>	location;
    Locations::iterator			it;
	
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
	//std::cout << "404 not found: " << _path << std::endl;
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
