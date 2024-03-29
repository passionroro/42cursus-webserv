#include "Response.hpp"
#include "Server.hpp"
#include "MimeTypes.hpp"
#include <dirent.h>
#include <sys/stat.h>

Response::Response(void)
{
}

Response::Response(std::string request, Server& server_conf) : Request(request, server_conf)
{
	_cgiDone = false;
	if (getStatus()[0] == '4')
        _locIndex = _locations.end();
	else if (getStatus()[0] == '3')
		redirectRequest();
	else if (_locIndex != _locations.end() && pathIsCGI(server_conf))
	{
		cgi(server_conf);
		_cgiDone = true;
	}
	else if (_isDir)
		directoryListing();
	else if(_method == "POST")
		postMethod(server_conf);
	else if(_method == "DELETE")
		deleteMethod();
	else
		readStaticPage();

	if (getStatus()[0] == '4' || getStatus()[0] == '5')
		readErrorPage(server_conf, getStatus());
	
	createHeaders();
	_status_code = getStatus();
	_status_text = getStatusText();
}

Response::~Response(void)
{
}

std::string Response::getUploadFilename() {

    std::string filename;
    size_t      pos;
    size_t      end_pos;

    filename.clear();
    pos = _requestBody.find(std::string("filename")) ;
    if (pos != std::string::npos) {
        pos += 10;
        end_pos = _requestBody.find('\"', pos);
        if (end_pos != std::string::npos)
            filename = _requestBody.substr(pos, end_pos - pos);
    }

    return filename;
}

void Response::eraseBodyBoundaries() {

    std::string::size_type bodyStart = 0;
	std::string boundary = _requestHeaders["Content-Type"].substr(_requestHeaders["Content-Type"].find('=') + 1);
    bodyStart = _requestBody.find(std::string("filename"), bodyStart);
    bodyStart = _requestBody.find(std::string("\r\n\r\n"), bodyStart);
    _requestBody.erase(0, bodyStart + 4);

    std::string::size_type bodyEnd = _requestBody.find(boundary);
    if (bodyEnd != std::string::npos)
        _requestBody.erase(bodyEnd, _requestBody.size());

	_requestBody.erase(_requestBody.find_last_of("\r\n") - 1, 2);
	_requestBody.erase(_requestBody.find_last_of("--") - 1, 2);
}

void Response::uploadFile(Server &server_conf) {

    std::vector<std::string> upload;
    std::string filename;
    size_t      max_body_size;
    size_t      content_length;

    // Get config file attributes for the /images folder
	upload = server_conf.getUploads();

    max_body_size = std::stoi(upload.back());
    content_length = std::stoi(_requestHeaders["Content-Length"]);
    if (content_length > max_body_size) {
        std::cerr << BOLD << RED << "Max upload file is " << max_body_size << " bytes." << DEFAULT << std::endl;
        setStatus("413");
        return ;
    }

    // File creation
    filename = getUploadFilename();
    if (filename.empty()) {
		std::cerr << BOLD << RED << "Failure opening upload file." << DEFAULT << std::endl;
        return ;
	}

    std::string		path = upload.front() + "/" + filename;
    std::ofstream	ofs(path, std::fstream::out | std::fstream::binary);

    if (!ofs.good() || !ofs.is_open()) {
        std::cerr << BOLD << RED << "Failure opening file at " << path << DEFAULT << std::endl;
        return ;
    }

    eraseBodyBoundaries();

    ofs.write(_requestBody.c_str(), _requestBody.size());

    ofs.close();
}

void Response::postMethod(Server &server_conf)
{
	if (_requestHeaders["Content-Type"].compare(0, 19, "multipart/form-data") == 0)
		uploadFile(server_conf);
    else {
		std::fstream inputstream;

		inputstream.open(_path,std::fstream::app);
		inputstream << _requestBody;
    }

}


void Response::deleteMethod()
{
	FILE * file = fopen(_path.c_str(),"r");
	if (file == NULL)
	{
		_status_code = "404";
	}
	else if ((remove(_path.c_str()) == 0))
		_status_code = ("200");
	else
	{
		_status_text = "Delete failed";
		_status_code = ("404");
	}
}

void	Response::getContentType(void)
{
	MimeTypes	mt;

	if (mt.getMap().find("html") == mt.getMap().end())
		return ;
	
	if (_cgiDone)
		_response_headers.insert(std::make_pair("Content-Type", mt.getMap()["html"]));

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
	texts.insert(std::make_pair("302", "Found"));
	texts.insert(std::make_pair("307", "Temporary Redirect"));
	texts.insert(std::make_pair("308", "Permanent Redirect"));
	texts.insert(std::make_pair("400", "Bad Request"));
	texts.insert(std::make_pair("404", "Not found"));
	texts.insert(std::make_pair("403", "Forbidden"));
	texts.insert(std::make_pair("405", "Method Not Allowed"));
    texts.insert(std::make_pair("409", "Conflict"));
    texts.insert(std::make_pair("413", "Request Entity Too Large"));
	texts.insert(std::make_pair("500", "Internal Server Error"));
	texts.insert(std::make_pair("501", "Not Implemented"));
	texts.insert(std::make_pair("508", "Loop Detected"));

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
	(void)size;
}

void	Response::appendHeaders(std::string & str)
{
	for (MapStr::iterator it = _response_headers.begin() ;
		it != _response_headers.end() ; it++)
	{
		str.append(it->first + ": " + it->second + "\r\n");
	}
	if (_cgiDone == false)
		str += "\r\n";
}

int	Response::readStaticPage(void)
{
	std::ifstream	file;
    std::stringstream	sstream;

	file.open(_path.c_str(), std::fstream::in);
	if (file.is_open())
	{
        sstream << file.rdbuf();
        _response_body = sstream.str();

		file.close();
		return (200);
	}
	else
	{
		std::cout << "bro nul" << std::endl;
		return (404);
	}
}

void	Response::redirectRequest(void)
{
	std::cout << "Redirect Request called" << std::endl;
	std::cout << "new_url: " << _newURL << std::endl;
	_response_headers.insert(std::make_pair("Location", _newURL));
	_response_headers.insert(std::make_pair("Connection", "keep-alive"));
}

void	Response::cgi(Server& server_conf)
{
	std::cout << "cgi called" << std::endl;

	Cgi	cgi(*this, server_conf);
	if (cgi.cgiFailed() == 1)
		setStatus("500");
	else if (cgi.cgiFailed() == 2)
		setStatus("508");
	else
		_response_body = cgi.getRes();
}

void	Response::directoryListing(void)
{
	if (_method == "DELETE" || !_autoIndex)
	{
		setStatus("403");
		return;
	}

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

	std::string path = getRequestPath();
	if (path.back() == '?')
		path.pop_back();

	std::string	html("<!DOCTYPE html>\r\n"
					"<html lang=\"en\">\r\n"
					"<head>\r\n"
					"<meta charset=\"UTF-8\">"
    				"<meta name=\"viewport\""
          			"content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">"
    				"<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">"
    				"<title>webserv - 42</title>"
    				"<link rel=\"stylesheet\" href=\"css/styles.css\" media=\"screen\">"
    				"<link rel=\"icon\" href=\"favicon.ico\">"
    				"<link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">"
    				"<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>"
    				"<link href=\"https://fonts.googleapis.com/css2?family=Montserrat:wght@300&display=swap\" rel=\"stylesheet\">"
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

bool	Response::pathIsCGI(Server& server_conf)
{
	std::string	extension;
	size_t	sep = _path.find_last_of('.');

	Locations const&	cgi = server_conf.getCgi();

	if (sep != _path.npos)
	{
		extension = _path.substr(sep, _path.back());
	}
	for (Locations::const_iterator it = cgi.begin() ; it != cgi.end() ; it++)
	{
		if (extension == it->at("extension"))
		{
			_cgiBin = it->at("bin");
			return true;
		}
	}
	return false;
}

void	Response::readErrorPage(Server& conf, std::string const & status)
{
	std::ifstream	    file;
	std::stringstream	sstream;
	std::string		    newPath;

	newPath = conf.getErrorPages()["path"] + conf.getErrorPages()[status];

	file.open(newPath.c_str(), std::fstream::in);
	if (file.is_open())
	{
		sstream << file.rdbuf();
		_response_body = sstream.str();
		file.close();
	}
	else
	{
		std::cout << "Error page not found" << std::endl;
	}
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
