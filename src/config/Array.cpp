#include "../../inc/Config.hpp"

/* ARRAY */
void Array::parseValue(std::string str, int *i) {

    if (str[*i] == '"') { /* parse string */
        std::string value = valueIsString(str, i);
        *i += 1;
        _string.push_back(value);
    }

    else if (isdigit(str[*i]) || str[*i] == '-') { /* parse int */
        int value = valueIsInt(str, i);
        _int.push_back(value);
    }

    else if (!str.compare(*i, 4, "true") || !str.compare(*i, 5, "false")) {
        _bool.push_back(valueIsBool(str, i));
    }

    else if (str[*i] == '[') { /* parse array */
        Array value = valueIsArray(str, i);
        _Array.push_back(value);
    }

    else if (str[*i] == '{') { /* parse object */
        Object value = valueIsObject(str, i);
        _Object.push_back(value);
    }

    else
        jsonError("incorrect value");
}
