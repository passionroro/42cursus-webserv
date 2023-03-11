#include "../../inc/Config.hpp"

/* OBJECT */
void Object::parseValue(std::string str, int *i, std::string key) {

    *i += 1;
    if (str[*i] != ':')
        jsonError("key/value pair must be separated by colon");
    *i += 1;

    if (str[*i] == '"') { /* parse string */
        std::string value = valueIsString(str, i);
        *i += 1;
        _string.insert(std::pair<std::string, std::string>(key, value));
    }

    else if (isdigit(str[*i]) || str[*i] == '-') { /* parse int */
        int value = valueIsInt(str, i);
        _int.insert(std::pair<std::string, int>(key, value));
    }

    else if (!str.compare(*i, 4, "true") || !str.compare(*i, 5, "false")) {
        _bool.insert(std::pair<std::string, bool>(key, valueIsBool(str, i)));
    }

    else if (str[*i] == '[') { /* parse array */
        Array value = valueIsArray(str, i);
        _Array.insert(std::pair<std::string, Array>(key, value));
    }

    else if (str[*i] == '{') { /* parse object */
        Object value = valueIsObject(str, i);
        _Object.insert(std::pair<std::string, Object>(key, value));
    }

    else
        jsonError("incorrect value");
}

std::string Object::parseKey(std::string str, int *i) {
    if (str[*i] != '"')
        jsonError("key must be a string");

    *i += 1;
    int start = *i;
    while (str[*i] != '"')
        *i += 1;
    if (start == *i)
        jsonError("key can not be empty");

    return (str.substr(start, *i - start));
}
