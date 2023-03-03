#include "../inc/webserv.hpp"
#include "request.hpp"
#include "response.hpp"
int main() {
	request req;
	std::string test = "GET /test.html HTTP/1.1\r\n"
					   "Host: origin.example.com\r\n"
					   "Content-Type: video/h264\r\n"
					   "Content-Length: 1234567890987\r\n"
					   "Expect: 100-continue\r\n";
	req.is_valid(test);
	response rep(req);
	std::cout <<  rep.send_response();
    return 1;
}
