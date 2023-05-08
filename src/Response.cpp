#include "Response.hpp"
#include <dirent.h>

Response::Response(void)
{
}

Response::Response(std::string request, Server& server_conf) : Request(request, server_conf)
{
	this->_status_code = "200";
	if (_status_code == "200")
		_status_text = "OK";

	createHeaders();
	if (_locations[0]["bin"] != "")
		cgi(server_conf);
	else if (1)
		directoryListing();
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

void	Response::cgi(Server& server_conf)
{
	std::cout << "cgi called" << std::endl;

	Cgi	cgi(*this, server_conf);
	_response_body = cgi.getRes();
}

void	Response::directoryListing(void)
{
	std::cout << "directory listing called" << std::endl;

	std::string	path = ".";
	DIR*	dir;
	struct dirent*				ent;
	std::vector<struct dirent>	entries;

	dir = opendir(path.c_str());
	if (!dir)
		return ;
	while ((ent = readdir(dir)) != NULL)
		entries.push_back(*ent);
	closedir(dir);

	std::sort(entries.begin(), entries.end(), comp);

	std::string	html("<!DOCTYPE html>\r\n"
					"<html lang=\"en\">\r\n"
					"<head>\r\n"
					"</head>\r\n"
					"<body>\r\n"
					"<h1>Index of " + path + "</h1>\r\n");


	for (unsigned long i = 0 ; i < entries.size() ; i++)
	{
		std::string line;

		line += "<a href=\"";
		line += path;
		if (path[path.size() - 1] != '/')
			line += '/';
		line += entries[i].d_name;
		line += "\">";
		line += entries[i].d_name;
		line += "</a></br>\r\n";
		html += line;
	}


	html += "</body>\r\n";
	_response_body += html;
}


std::string	Response::renderString(void)
{
	std::string	str;
	str= "HTTP/1.1 " + _status_code + " " + _status_text  + "\r\n";
	appendHeaders(str);
	str += _response_body;
	return (str);
}

bool	comp(struct dirent x, struct dirent y)
{
	return (std::strcmp(x.d_name, y.d_name) > 0);
}
