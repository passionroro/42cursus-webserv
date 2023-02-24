#include "../inc/webserv.hpp"
#include "request.hpp"
#include "response.hpp"
int main() {
	request req;
	std::string test = "GET /test.html HTTP/1.1\r"
					   "Host: origin.example.com\n"
					   "Content-Type: video/h264\n"
					   "Content-Length: 1234567890987\n"
					   "Expect: 100-continue\n";
	response rep(req.is_valid(test));
	std::cout <<std::endl <<  rep.send_response();
    return 1;
}
