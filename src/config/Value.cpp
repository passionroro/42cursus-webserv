#include "Config.hpp"

/* VALUE */
std::string Value::valueIsString(std::string str, int *i) {
    *i += 1;
    int start = *i;
    while (str[*i] != '"')
        *i += 1;

    return (str.substr(start, *i - start));
}

int Value::valueIsInt(std::string str, int *i) {
    int start = *i;
    while ((str[*i] == '-' && start == *i) || isdigit(str[*i]))
        *i += 1;
    std::string value = str.substr(start, *i - start);

    return (atoi(value.c_str()));
}

bool Value::valueIsBool(std::string str, int *i) {
    if (str.compare(*i, 4, "true") == 0) {
        *i += 4;
        return (true);
    }
    *i += 5;

    return (false);
}

Array Value::valueIsArray(std::string str, int *i) {
    Array arr;
    *i += 1;
    while (str[*i] && str[*i] != ']') {
        arr.parseValue(str, i);
        if (str[*i] == ',' || str[*i] == ']') {
            *i += 1;
            if (str[*i - 1] == ']')
                break ;
        }
        else
            jsonError("array: missing separator.");
    }
    return arr;
}

Object Value::valueIsObject(std::string str, int *i) {
    *i += 1;
    Object obj;
    std::string key;
    while (str[*i] && str[*i] != '}') {
        key = obj.parseKey(str, i);
        obj.parseValue(str, i, key);
        if (str[*i] == ',' || str[*i] == '}') {
            *i += 1;
            if (str[*i - 1] == '}')
                break ;
        }
        else
            jsonError("object: missing separator.");
    }
    return obj;
}

void	Value::jsonError(std::string msg)
{
	std::string ret = "json format error: ";
	ret.append(msg);
	std::cout << ret.c_str() << std::endl;
	exit(EXIT_FAILURE);
}
