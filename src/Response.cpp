#include "Response.hpp"
#include "MimeTypes.hpp"
#include <dirent.h>

Response::Response(void)
{
}

Response::Response(std::string request, Server& server_conf) : Request(request, server_conf)
{

	if (getStatus() != "200")
	{
		_path = "home/www/error_404.html";
		//std::cout << "new path is error_404" << std::endl;
		_locIndex = _locations.end();
	}

	if (_locIndex != _locations.end() && (*_locIndex)["bin"] != "")
		cgi(server_conf);
	else if (_isDir)
		directoryListing();
	else if(_method == "POST")
		postMethod();
	else if(_method == "DELETE")
		deleteMethod();
	else
		readStaticPage();
	
	createHeaders();
	_status_code = getStatus();
	_status_text = getStatusText();
	return;
}

Response::~Response(void)
{
}

// get the file name: Content-Disposition: form-data; name="uploaded_file"; filename="Screen Shot 2023-05-16 at 2.56.32 PM.png"
// get the folder in locations
// create a file in /images
// get the file Content-Length: 1112710
// get the clt_max_body_size in locations
// read into a buffer
// append into the new file

bool Response::uploadFile() {

    Locations::iterator upload;

    for (upload = _locations.begin(); upload != _locations.end(); upload++) {
        if (upload->at("path") == "/images") {
            break ;
        }
    }

    if (upload == _locations.end())
        return false;

    std::string   max_body_size = (*upload)["client_max_body_size"];
    std::string    path = (*upload)["path"];
//    size_t              content_length;
    std::string         filename;
    size_t pos = 0;

    pos = _request_body.find(std::string("filename"));
    if (pos == std::string::npos)
        return false;
	
	pos += 10;
	
	size_t endpos = _request_body.find('\"', pos);
    if (endpos == std::string::npos)
        return false;

    filename = _request_body.substr(pos, endpos - pos);

    return true;

}

void Response::postMethod()
{
    MapStr::iterator    it;
    bool                upload = false;

	if (_request_headers["Content-Type"].compare(0, 19, "multipart/form-data") == 0)
		upload = uploadFile();

	//check return values instead of boolean
	
    if (upload == false) {
		std::fstream inputstream;

		inputstream.open(_path,std::fstream::app);
		inputstream << _request_body;
    }

//	if (_request_headers["Content-Type"].second.compare(0, 19, "multipart/form-data")) {
//		uploadFile();
//	}
//	else
//	{
//		std::fstream inputstream;
//
//		inputstream.open(_path,std::fstream::app);
//		inputstream << _request_body;
//	}
}

//void Response::uploadImage()
//{
//	std::vector<char> buffer;
//
//	FILE* file_stream = fopen(_path.c_str(), "rb");
//
//	std::string file_str;
//
//	size_t file_size;
//	if (file_stream != nullptr) {
//		fseek(file_stream, 0, SEEK_END);
//		long file_length = ftell(file_stream);
//		rewind(file_stream);
//
//		buffer.resize(file_length);
//
//		file_size = fread(&buffer[0], 1, file_length, file_stream);
////		if (buffer.back() != nullptr) {
////			file_size = fread(buffer, 1, file_length, file_stream);
////
////			std::stringstream out;
////
////			for (size_t i = 0; i < file_size; i++) {
////				out << buffer[i];
////			}
////
////			std::string copy = out.str();
////
////			file_str = copy;
////		}
////	}
////
////	if(file_str.length() > 0)
////	{
////		std::string file_size_str = std::to_string(file_str.length());
////
////		std::string html = "HTTP/1.1 200 Okay\r\nContent-Type: image/png; Content-Transfer-Encoding: binary; Content-Length: " + file_size_str + ";charset=ISO-8859-4 \r\n\r\n" + file_str;
////
////		printf("\n\nHTML -> %s\n\nfile_str -> %ld\n\n\n", html.c_str(), file_str.length());
////	}
////
//	}
//}


void Response::deleteMethod()
{
	if ((remove(_path.c_str()) == 0))
		_status_code = ("204");
	else
	{
		_status_text = "Delete failed";
		_status_code = ("200");
	}
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





	//std::cout << "mimetype: " << mt.getMap()["html"] << std::endl;
	//_response_headers.insert(std::make_pair("Content-Type", mt.getMap()["html"]));
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
	(void)size;
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
