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

Response::Response(std::string request, Server& server_conf) : Request(request, server_conf)
{
	this->_status_code = "200";
	if (_status_code == "200")
		_status_text = "OK";

	createHeaders();
	std::cout << "yoooooo:" << _locations[1]["cgi"] << std::endl;
	if (_locations[1]["cgi"] != "")
		cgi();
	else
		readStaticPage();
	return;
}

Response::~Response(void)
{
}

//std::string	Response::buildPath(void)
//{
//}

void	Response::createHeaders(void)
{
	int size;
	char buf[100];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	tm.tm_zone = (char *)"GMT";
	size = ::strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z\n", &tm);
	_response_headers.insert(std::pair<std::string, std::string>("Date", buf));
	(void)size;
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

void	Response::cgi(void)
{
	std::cout << "cgi yaaaaaasss" << std::endl;

	Cgi	cgi(*this);
	_response_body = cgi.getRes();
}

std::string	Response::renderString(void)
{
	std::string	str;
	str= "HTTP/1.1 " + _status_code + " " + _status_text  + "\r\n";
	appendHeaders(str);
	str += _response_body;
	return (str);
}
