# Config file
## To do
### Refactor
* `class Config` : config file parsing. maybe assign the vector of Servers here.
```c++ 
void parse(void);
void vectorize(void);
void trim(void);
```
    
* `class Value` : accessing the methods to parse any kind of value.
```c++ 
std::string valueIsString(std::string str, int *i);
int         valueIsInt(std::string str, int *i);
bool        valueIsBool(std::string str, int *i);
Array       valueIsArray(std::string str, int *i);
Object      valueIsObject(std::string str, int *i);
```

* `class Array : protected Value`
```c++
protected:
    void    parseValue(std::string str, int *i);
private:
    std::vector<std::string>  _string;
    std::vector<int>          _int;
    std::vector<bool>         _bool;
    std::vector<Object>       _Object;
    std::vector<Array>        _Array;
```

* `class Object : protected Value`
```c++
protected:
    std::string parseKey(std::string str, int *i);
    void        parseValue(std::string str, int *i, std::string key);
private:
    std::map<std::string, std::string>  _string;
    std::map<std::string, int>          _int;
    std::map<std::string, bool>         _bool;
    std::map<std::string, Object>       _Object;
    std::map<std::string, Array>        _Array;
```
