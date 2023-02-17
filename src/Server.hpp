#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Config.hpp"

class	Server {

public:

	Server(std::string const & config_file);

	void	run(void);


private:

	Config	_config;

	Server(void);

};

#endif
