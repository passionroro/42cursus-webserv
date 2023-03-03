#ifndef CONFIG_PARSING_HPP
# define CONFIG_PARSING_HPP

//# include "../inc/webserv.hpp"
# include "Config.hpp"

//class Config;
class Array;
class Object;


/* VALUE METHODS */
class Value : public Config {
public:
    std::string valueIsString(std::string str, int *i);
    int         valueIsInt(std::string str, int *i);
    bool        valueIsBool(std::string str, int *i);
    Array       valueIsArray(std::string str, int *i);
    Object      valueIsObject(std::string str, int *i);
};


/* ARRAY */
class Array : public Value {
public:
    void    parseValue(std::string str, int *i);
private:
    std::vector<std::string>  _string;
    std::vector<int>          _int;
    std::vector<bool>         _bool;
    std::vector<Object>       _Object;
    std::vector<Array>        _Array;
};


/* OBJECT */
class Object : public Value {
public:
    Object() {};
    ~Object() {};
//    void        parseObject(std::string str, int *i);
    std::string parseKey(std::string str, int *i);
    void        parseValue(std::string str, int *i, std::string key);
private:
    std::map<std::string, std::string>  _string;
    std::map<std::string, int>          _int;
    std::map<std::string, bool>         _bool;
    std::map<std::string, Object>       _Object;
    std::map<std::string, Array>        _Array;
};

#endif
