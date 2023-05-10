#ifndef MIME_TYPES_HPP
# define MIME_TYPES_HPP

# include "../inc/webserv.hpp"

class	MimeTypes {

public:

	MimeTypes(void);
	virtual ~MimeTypes(void);

private:

	std::map<std::string, std::string>	_map;


};


#endif
