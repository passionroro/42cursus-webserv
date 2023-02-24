#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../inc/webserv.hpp"
# include "Server.hpp"

class	Config {

public:

    Config(void);
    ~Config(void);

    void	setup(std::string const & file);

protected:
    void    parse(void);
	void	vectorize(void);
    void    trim(void);
    void    debug(void);


private:
    std::ifstream       ifs;
    std::string         _file;
	std::vector<std::string>	cfg;
	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	ite;
};

#endif
