#ifndef MIME_TYPES_HPP
# define MIME_TYPES_HPP

# include "../inc/webserv.hpp"

class	MimeTypes {

public:

	MimeTypes(void);
	virtual ~MimeTypes(void);

	MapStr&	getMap(void);

private:

	MapStr	_map;


};


#endif
