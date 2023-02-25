#ifndef OBJECT_HPP
# define OBJECT_HPP

# include "../inc/webserv.hpp"
# include "Config.hpp"

class Object {
public:
    Object() {};
private:
    std::map<std::string, std::string>  obj_string;
    std::map<std::string, int>          obj_int;
    std::map<std::string, bool>         obj_bool;
    std::map<std::string, Object>       obj_Object;
    std::map<std::string, Array>        obj_Array;
};

#endif
