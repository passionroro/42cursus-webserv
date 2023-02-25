#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../inc/webserv.hpp"
# include "Server.hpp"

class Array;
class Object;

class	Config {

public:
    Config(void);
    ~Config(void);
    void	setup(std::string const & file);


protected:
    void        parse(void);
    void	    vectorize(void);
    void        trim(void);

    std::string parseKey(std::string str, int *i);
    void        parseValue(std::string str, int *i, std::string key);

    std::string valueIsString(std::string str, int *i);
    int         valueIsInt(std::string str, int *i);
    bool        valueIsBool(std::string str, int *i);
    Array       valueIsArray(std::string str, int *i);
    Object      valueIsObject(std::string str, int *i);

    void        debug(void);
    void        jsonError(std::string msg);


private:
    std::string         _file;
    std::string         str;
    std::map<std::string, std::string>  m_string;
    std::map<std::string, int>          m_int;
    std::map<std::string, bool>         m_bool;
    std::map<std::string, Object>       m_Object;
    std::map<std::string, Array>        m_Array;
};



#endif
