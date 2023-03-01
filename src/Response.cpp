#include "Response.hpp"

Response::Response(void)
{
	createHeaders();
	_path = "./index.html";
	readStaticPage();
}

Response::~Response(void)
{
}

void	Response::createHeaders(void)
{
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
	str += "\r\n";
}

int	Response::readStaticPage(void)
{
	std::ifstream	file;
	std::stringstream	sstream;

	file.open(_path.c_str(), std::fstream::in);
	if (file.is_open() == true)
	{
		sstream << file.rdbuf();
		_page = sstream.str();
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

	str = "HTTP/1.0 200 OK\r\n";
	appendHeaders(str);
	//str += "It works!";
	str += _page;
	return (str);
}
