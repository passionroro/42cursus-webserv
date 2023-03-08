#include "Response.hpp"

Response::Response(void)
{
}

Response::Response(std::string request) : Request(request)
{
	_status_code = "200";
	_code = 200; // todo stoi
	if (_code == 200)
		_status_text = "OK";
	//_path = request.getPath();

	(void)request;
	_path = "./home/www/index.html";
	createHeaders();
	//readStaticPage();
	process();
	return;
}

Response::~Response(void)
{
}

void	Response::process(void)
{
	if (_method == "GET")
		handleGet();
	else if (_method == "POST")
		handlePost();
	else if (_method == "DELETE")
		handleDelete();
	else
		std::cout << "Unknown method" << std::endl; // todo?
}

void	Response::handleGet(void)
{
	std::cout << "Handle GET" << std::endl;

	if (isCgi())
	{
		// todo handle cgi
		std::cout << "path is CGI" << std::endl;
	}
	else if (_code == 200)
		_code = readStaticPage();
	else
		_code = readDefaultPage(_code);
	if (_code == 404)
		_code = readDefaultPage(_code);



}

void	Response::handlePost(void)
{
	std::cout << "Handle POST" << std::endl;
}

void	Response::handleDelete(void)
{
	std::cout << "Handle DELETE" << std::endl;
}

bool	Response::isCgi(void)
{
	return (false);
}

void	Response::createHeaders(void)
{
	int size;
	char buf[100];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	tm.tm_zone = (char *)"GMT";
	size = ::strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z\n", &tm);
	_response_headers.insert(std::pair<std::string, std::string>("Date: ", buf));
	// exemple
	//_headers.insert(std::make_pair("Content-Length", "9"));
}

void	Response::appendHeaders(std::string & str)
{
	for (std::map<std::string, std::string>::iterator it = _response_headers.begin() ;
		it != _response_headers.end() ; it++)
	{
		str.append(it->first + ": " + it->second + "\r\n");
	}
	//str += "\r\n";
}

int	Response::readStaticPage(void)
{
	std::ifstream	file;
	std::stringstream	sstream;

	file.open(_path.c_str(), std::fstream::in);
	if (file.is_open() == true)
	{
		sstream << file.rdbuf();
		_response_body = sstream.str();
		file.close();
		//std::cout << "PAGE:" << std::endl << _page << std::endl;
		return (200);
	}
	else
	{
		std::cout << "bro nul" << std::endl;
		//todo default error page 404
		return (404);
	}
}

int	Response::readDefaultPage(int code)
{
	int	res;
	std::string	cpy = _path;

	switch (code)
	{
		case 400:
			_path = "home/error/400.html"; break;
		case 403:
			_path = "home/error/403.html"; break;
		case 404:
			_path = "home/error/404.html"; break;
		case 405:
			_path = "home/error/405.html"; break;
		case 410:
			_path = "home/error/410.html"; break;
		case 413:
			_path = "home/error/413.html"; break;
		case 500:
			_path = "home/error/500.html"; break;
		default:
			return (404);
	}
	res = readStaticPage();
	_path = cpy;
	return (res);
}

std::string	Response::renderString(void)
{
	std::string	str;
	str= "HTTP/1.1 " + _status_code + " " + _status_text  + "\r\n";
	//str = "HTTP/1.0 200 OK\r\n";
	appendHeaders(str);
	//str += "It works!";
	str += _response_body;
	return (str);
}
