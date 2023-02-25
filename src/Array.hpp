#ifndef ARRAY_HPP
# define ARRAY_HPP

# include "../inc/webserv.hpp"
# include "Config.hpp"

class Array {
public:
    Array() {};
private:
    std::vector<std::string>  arr_string;
    std::vector<int>          arr_int;
    std::vector<bool>         arr_bool;
    std::vector<Object>       arr_Object;
    std::vector<Array>        arr_Array;
};

#endif
