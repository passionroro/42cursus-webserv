#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "webserv.hpp"

class Config;
class Array;
class Object;

class	Config {
public:
    Config(void);
    ~Config(void);
    Object	setup(std::string const & file);

protected:
    std::string         str;
    void                debug(void);
    void                jsonError(std::string msg);

private:
    std::string _file;
    Object      parse(void);
    void	    vectorize(void);
    void        trim(void);
};


/* VALUE METHODS */
class Value : protected Config {
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
    std::string parseKey(std::string str, int *i);
    void        parseValue(std::string str, int *i, std::string key);

    std::map<std::string, std::string>  getString() { return _string; };
    std::map<std::string, int>          getInt() { return _int; };
    std::map<std::string, bool>         getBool() { return _bool; };
    std::map<std::string, Object>       getObject() { return _Object; };
    std::map<std::string, Array>        getArray() { return _Array; };

private:
    std::map<std::string, std::string>  _string;
    std::map<std::string, int>          _int;
    std::map<std::string, bool>         _bool;
    std::map<std::string, Object>       _Object;
    std::map<std::string, Array>        _Array;
};

#endif
