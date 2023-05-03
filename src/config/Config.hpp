#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "webserv.hpp"

class Config;
class Array;
class Object;

/* VALUE METHODS */
class Value {
public:
	void	jsonError(std::string msg);

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
    
	std::vector<std::string>&  getString() { return _string; };
    std::vector<int>&          getInt() { return _int; };
    std::vector<bool>&         getBool() { return _bool; };
    std::vector<Object>&       getObject() { return _Object; };
    std::vector<Array>&        getArray() { return _Array; };
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

    std::map<std::string, std::string>  &getString() { return _string; };
    std::map<std::string, int>          getInt() { return _int; };
    std::map<std::string, bool>         getBool() { return _bool; };
    std::map<std::string, Object>       getObject() { return _Object; };
    std::map<std::string, Array>        getArray() const { return _Array; };

private:
    std::map<std::string, std::string>  _string;
    std::map<std::string, int>          _int;
    std::map<std::string, bool>         _bool;
    std::map<std::string, Object>       _Object;
    std::map<std::string, Array>        _Array;
};

class	Config {
public:
    Config(void);
    Config(int argc);
    ~Config(void);
    void	setup(std::string const & file);

	const Object getData(void) const;

    bool        overwrite;

protected:
    std::string         str;
    void                jsonError(std::string msg);

private:

	Object		_data;
    std::string _file;
    Object      parse(void);
    void	    vectorize(void);
    void        trim(void);
};


#endif
