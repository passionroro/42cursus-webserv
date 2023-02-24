#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../inc/webserv.hpp"
# include "Server.hpp"

class	Config {

public:

    Config(void);
    ~Config(void);

    void	setup(std::string const & file);
    void    parse(void);


private:
    std::ifstream       ifs;
    std::string         _file;

//    std::ifstream   _stream;
//    std::string     _filename;
//    std::vector<Server> _servers;
};

#endif
