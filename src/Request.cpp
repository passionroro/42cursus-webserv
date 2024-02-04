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
	_disabledMethod = server_conf.getDisabledMethods();
	parseRequest(request, server_conf);
	_autoIndex = server_conf.getAutoIndex();
}

Request::~Request() {}

/* MEMBER FUNCTIONS */

int Request::parseRequest(std::string &request, Server& conf)
{
	std::vector<std::string> firstLine;
	std::string R_line;
	int spaceCount = 0;
	setStatus("200");
	
	firstLine = split(request.substr(0,request.find('\r')), ' ');
	if (firstLine.size() != 3) {
		setStatus("400");
		return 0;
	}
	
	R_line = request.substr(0, request.find('\r'));
  
	for (int i = 0; i < (int)R_line.size(); ++i) {
		if(isspace(R_line[i]))
			spaceCount++;
	}
	if(spaceCount != 2)
		setStatus("400");
	
	_method = firstLine.front();
	checkMethod();
	if (_status[0] != '2')
		return 0;
	
	_path = firstLine.at(1);
	_requestPath = _path;
    if (_path.find('?') != std::string::npos){
        _path = _path.substr(0, _path.find('?'));
    }
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
	return 0;
}

void	Request::checkRedirection(Server& conf)
{
	Redirection const& redirection = conf.getRedirection();

    Redirection::const_iterator it;
	
	for (it = redirection.begin() ; it != redirection.end(); it++)
	{
        if (_path == it->at("old_url") && it->at("type") == "permanent")
		{
			_newURL = it->at("new_url");
			_status = "301";
			return ;
		}
        if (_path == it->at("old_url") && it->at("type") == "tmp")
		{
			_newURL = it->at("new_url");
			_status = "302";
			return ;
		}
    }
}

int Request::parseHeaders(std::string &request) {

	std::string h_key;
	std::string h_value;
	size_t pos = 0;
	size_t pos_2 = 0;
	while ((pos = request.find(':')) != std::string::npos) {
		if (request.find("\r\n", 0) == 0)
			break;
		h_key = (request.substr(0, pos));
		request.erase(0, pos + 1);
		pos_2 = request.find('\r');
		h_value = (request.substr(0, pos_2));
  
		if (h_value[0] == ' ')
			h_value.erase(0, 1);
		request.erase(0, pos_2 + 2);
		if (h_key.find(' ') != std::string::npos)
      		setStatus("400");
    	_requestHeaders.insert(std::pair<std::string, std::string>(h_key, h_value));
	}
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

	if (_requestHeaders.find("Host") == _requestHeaders.end())
	{
		_status = "400";
		return ;
	}
	std::string host;
	host = _requestHeaders.at("Host");

	size_t sep;
	if ((sep = host.find_first_of(':')) != host.npos)
		host = host.substr(0, sep);


	if (host == conf.getServerName())
		return ;

	std::vector<std::string>& addr = conf.getAddress();
	for (unsigned long i = 0 ; i < addr.size() ; i++)
	{
		if (host == addr[i])
			return ;
	}
	std::cout << "Bad host, bad request" << std::endl;
	_status = "400";
}

void Request::parseBody(std::string &request) {
	request.erase(0,2);
	int i;
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
	{
		_requestBody = request;
	}
}

void Request::checkMethod()
{
	std::vector<std::string> NotImplemented = split("CONNECT HEAD OPTIONS PUT TRACE PATCH",' ');
	
	for (std::vector<std::string>::iterator it = NotImplemented.begin();it != NotImplemented.end(); it++)
	{
		if (_method == *it)
		{
			setStatus("501");
			return ;
		}
	}
	for (std::vector<std::string>::iterator it = _disabledMethod.begin() ; it != _disabledMethod.end() ; it++)
	{
		if (_method == *it)
		{
			setStatus("405");
			return ;
		}
	}
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		setStatus("400");
}

void Request::checkPath()
{
	std::fstream fs;
	std::vector<std::string> location;
    Locations::iterator it;
	
	if (_path.size() != 1 && _path.back() == '/')
		_path.pop_back();
    for (it = _locations.begin();it != _locations.end(); it++)
	{
		if (_path.substr(0, _path.find('/',1)) == it->at("path")
                && _path != it->at("path"))
            _path = _path.substr(_path.find('/',1), std::string::npos);
        if (_path == it->at("path"))
            break;
    }
	
	if (it != _locations.end()) {
		_path.clear();
		_path = it->at("root");
		if (_path.back() != '/')
			_path.append("/");
        _path.append(it->at("index"));
        fs.open(_path);
        if (fs.is_open())
		{
			_locIndex = it;
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
				_locIndex = it;
				break;
			}
		}
		if (!fs.is_open() && _isDir == false)
			setStatus("404");
	}
}

/* SETTERS */
void Request::setStatus(std::string statusCode) {
    this->_status = statusCode;
}

/* ACCESSORS */
std::string Request::getStatus() const { return _status; }
std::string Request::getRequestPath() const { return _requestPath; }

/* PRIVATE CONSTRUCTOR */
