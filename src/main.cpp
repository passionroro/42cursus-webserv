#include "../inc/webserv.hpp"
#include "request.hpp"
int main() {
	request r;
	std::string test = "GET /test.html HTTP/1.1\r"
					   "Host:origin.example.com\n"
					   "Content-Type : video/h264\n"
					   "Content-Length: 1234567890987\n"
					   "Expect: 100-continue\n";
	r.is_valid(test);
    return 1;
}
