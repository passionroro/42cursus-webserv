#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "webserv.hpp"

class	Config {

public:

	Config(void);
	~Config(void);

	void	parse(std::string const & file);


private:

};

#endif
