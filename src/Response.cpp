#include "Response.hpp"

Response::Response(void)
{
}

/*Response::Response(void)
{
	createHeaders();
	_path = "./index.html";
	_path = "./index.html";
	readStaticPage();
}*/

Response::Response(Request const & request) : _version("HTTP/1.1 ")
{
	this->_status_code = "200";
	if (_status_code == "200")
	_status_text = "OK";
	//_path = request.getPath();

	(void)request;
	_path = "./home/www/index.html";
	createHeaders();
	readStaticPage();
	return;
}

Response::~Response(void)
{
}

void	Response::createHeaders(void)
{
	int size;
	char buf[100];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	tm.tm_zone = (char *)"GMT";
	size = ::strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z\n", &tm);
	_headers.insert(std::pair<std::string, std::string>("Date: ", buf));
	// exemple
	//_headers.insert(std::make_pair("Content-Length", "9"));
}

void	Response::appendHeaders(std::string & str)
{
	for (std::map<std::string, std::string>::iterator it = _headers.begin() ;
		it != _headers.end() ; it++)
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
		_body = sstream.str();
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

std::string	Response::renderString(void)
{
	std::string	str;
	str= _version + _status_code + " " + _status_text  + "\r\n";
	//str = "HTTP/1.0 200 OK\r\n";
	appendHeaders(str);
	//str += "It works!";
	str += _body;
	return (str);
}
