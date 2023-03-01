#include "Response.hpp"

Response::Response(void)
{
}

Response::~Response(void)
{
}

std::string	Response::createString(void)
{
	std::string	str;

	str = "HTTP/1.0 200 OK\r\n";
	str += "Content-Length: 9\r\n\r\n";
	str += "It works!";
	return (str);
}
