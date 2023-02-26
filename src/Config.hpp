#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../inc/webserv.hpp"

class Server;
class Value;
class Object;

class	Config {

public:
    Config(void);
    ~Config(void);
    void	setup(std::string const & file);

protected:
    std::string         str;
    std::vector<Object> servers;
    void                debug(void);
    void                jsonError(std::string msg);


private:
    std::string _file;
    void        parse(void);
    void	    vectorize(void);
    void        trim(void);
};

#endif
