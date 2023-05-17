#include "Response.hpp"
#include "MimeTypes.hpp"
#include <dirent.h>

Response::Response(void) {}
Response::~Response(void) {}

Response::Response(std::string request, Server& server_conf) : Request(request, server_conf)
{

	if (getStatus() != "200") {
		_path = "home/www/error_pages/custom_404.html";
		_locIndex = _locations.end();
	}

	if (_locIndex != _locations.end() && (*_locIndex)["bin"] != "")
		cgi(server_conf);
	else if (_isDir)
		directoryListing();
	else
		readStaticPage();
	
	createHeaders();

	_status_code = getStatus();
	_status_text = getStatusText();

    return;

}

void	Response::getContentType(void)
{
	MimeTypes	mt;

	if (mt.getMap().find("html") == mt.getMap().end())
		return ;
	
	std::string	extension;

	size_t	start = _path.find_last_of('.');
	if (start == _path.npos || start == _path.size() - 1)
		return ;
	extension = _path.substr(start + 1);
	_response_headers.insert(std::make_pair("Content-Type", mt.getMap()[extension]));

}

void	Response::getContentLength(void)
{
	std::stringstream	sstream;
	std::string	len;

	sstream << _response_body.size();
	sstream >> len;

	_response_headers.insert(std::make_pair("Content-Length", len));
}

std::string	Response::getStatusText(void)
{
	MapStr	texts;

	texts.insert(std::make_pair("200", "OK"));
	texts.insert(std::make_pair("201", "Created"));
	texts.insert(std::make_pair("301", "Moved Permanently"));
	texts.insert(std::make_pair("400", "Bad Request"));
	texts.insert(std::make_pair("404", "Not found"));
	texts.insert(std::make_pair("405", "Method Not Allowed"));
	texts.insert(std::make_pair("409", "Conflict"));
	texts.insert(std::make_pair("500", "Internal Server Error"));
	texts.insert(std::make_pair("501", "Not Implemented"));

	return (texts[getStatus()]);

}

void	Response::createHeaders(void)
{
	int size;
	char buf[100];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	tm.tm_zone = (char *)"GMT";
	size = ::strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response_headers.insert(std::make_pair("Server", "42-WebServ"));
	_response_headers.insert(std::pair<std::string, std::string>("Date", buf));

	getContentType();
	getContentLength();

}

void	Response::appendHeaders(std::string & str)
{
	for (MapStr::iterator it = _response_headers.begin() ;
		it != _response_headers.end() ; it++)
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

	//std::string	path = ".";
	DIR*	dir;
	struct dirent*				ent;
	std::vector<struct dirent>	entries;

	dir = opendir(_path.c_str());
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
					"<h1>Index of " + getRequestPath() + "</h1>\r\n");

	for (unsigned long i = 0 ; i < entries.size() ; i++)
	{
		std::string line;

		line += "<a href=\"";
		line += getRequestPath();
		if (getRequestPath()[getRequestPath().size() - 1] != '/')
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
	_response_head = "HTTP/1.1 " + _status_code + " " + _status_text  + "\r\n";
	appendHeaders(_response_head);
	str = _response_head + _response_body;
	return (str);
}

bool	comp(struct dirent x, struct dirent y)
{
	return (std::strcmp(x.d_name, y.d_name) > 0);
}
