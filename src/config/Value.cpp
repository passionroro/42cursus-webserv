#include "../../inc/Config.hpp"

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
    while (str[*i]) {
        arr.parseValue(str, i);
        i++;
    }
    return arr;
}

Object Value::valueIsObject(std::string str, int *i) {
    *i += 1;
    Object obj;
    std::string key;
    while (str[*i]) {
        key = obj.parseKey(str, i);
        obj.parseValue(str, i, key);
//        i++;
    }
    return obj;
}
