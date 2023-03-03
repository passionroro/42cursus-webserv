#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

class Request {
public:
	
	Request();
	Request(std::string request);
	~Request();
	
	int 	is_valid(std::string &Request);
	bool	check_path(std::string s);
	int 	parse_headers(std::string &Request);

	std::string getStatus() const;
	std::string getBody() const;
	std::string getPath() const;


private:
	std::string 						_method;
	std::string 						_path;
	std::string 						_version;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	std::string 						_status;
};

#endif
