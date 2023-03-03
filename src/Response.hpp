#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

class	Response {

public:

	Response(void);
	virtual	~Response(void);
	
	std::string	createString(void);

private:

};

#endif
