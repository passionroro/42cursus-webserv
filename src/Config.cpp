#include "Config.hpp"

/* CONSTRUCTOR */
Config::Config(void) {}

/* OVERLOAD */

/* FUNCTIONS */
void	Config::setup(std::string const & filename)
{
    if (filename.compare(filename.size() - 4, 4, "json"))
        jsonError("file extension must be json");

    std::ifstream       ifs;
    ifs.open(filename.c_str(), std::ifstream::in);
    if (!ifs.is_open())
        error("open()");

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    _file = str;
    parse();

    ifs.close();
}

/* just like ft_split (note: maybe use namespace utils for this function) */
void Config::vectorize() {
	int start;
    int end = 0;
    bool begin = true;
    while (end != -1) {
        start = end + 1;
        if (begin)
            start = 0;
        begin = false;
		end = _file.find('\n', start);
        std::string tmp = _file.substr(start, end - start);
		if (!tmp.empty())
            str.append(tmp);
	}
}

/* remove all spaces (also could use namespace::utils) */
void Config::trim() {
    size_t pos;
    while ((pos = str.find(' ')) != std::string::npos)
        str.erase(pos, 1);
}

void Config::debug() {
    std::cout << "|" << str << "|" << std::endl;
}

std::string Config::valueIsString(std::string str, int *i) {

    int start = *i;
    while (str[*i] != '"')
        *i += 1;
    if (start == *i)
        jsonError("value can not be empty");
    else if (str[*i + 1] != ',' && str[*i + 1] != '}' && str[*i + 1] != ']')
        jsonError("string value is either incorrect or not closed properly");

    return (str.substr(start, *i - start));
}

int Config::valueIsInt(std::string str, int *i) {

    int start = *i;
    while (isdigit(str[*i]))
        *i += 1;
    if (str[*i + 1] != ',' && str[*i + 1] != '}' && str[*i + 1] != ']')
        jsonError("integer value is either incorrect or not closed properly");

    std::string value = str.substr(start, *i - start);
    return (atoi(value.c_str()));
}

bool Config::valueIsBool(std::string str, int *i) {
    if (str.compare("true", *i, 4)) {
        *i += 4;
        return (true);
    }
    *i += 5;
    return (false);
}

/* An array is an ordered collection of values.
 * An array begins with [ and ends with ].
 * Values are separated by a comma.*/
Array Config::valueIsArray(std::string str, int *i) {
    (void)str, i;
}

/* An object is an unordered set of name/value pairs.
 * An object begins with { and ends with }.
 * Each name is followed by a colon and the name/value pairs are separated by a comma.*/
Object Config::valueIsObject(std::string str, int *i) {
    (void)str, i;
}

/* A value can be a string in double quotes, or a number, or true or false or null, or an object or an array.
 * These structures can be nested.*/
void Config::parseValue(std::string str, int *i, std::string key) {
    *i += 1;
    if (str[*i] != ':')
        jsonError("key/value pair must be separated by colon");
    *i += 1;

    if (str[*i] == '"') { /* parse string */
        std::string value = valueIsString(str, i);
        m_string.insert(std::pair<std::string, std::string>(key, value));
    }

    else if (isdigit(str[*i])) { /* parse int */
        int value = valueIsInt(str, i);
        m_string.insert(std::pair<std::string, int>(key, value));
    }

    else if (str.compare("true", *i, 4) || str.compare("false", *i, 5))
        m_bool.insert(std::pair<std::string, bool>(key, valueIsBool(str, i)));

    else if (str[*i] == '[') { /* parse array */
        Array value = valueIsArray(str, i);
        m_Array.insert(std::pair<std::string, Array>(key, value));
    }

    else if (str[*i] == '{') { /* parse object */
        Object value = valueIsObject(str, i);
        m_Object.insert(std::pair<std::string, Object>(key, value));
    }

    else
        jsonError("incorrect value");
}

std::string Config::parseKey(std::string str, int *i) {
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

void Config::parse() {

    vectorize();
    trim();
//    debug();

    if (str[0] != '{')
        jsonError("opening brackets");

    std::string key;
    int i = 0;
    while (str[++i]) {
        key = parseKey(str, &i);
        parseValue(str, &i, key);
        if (str[i] != ',' && str[i] != '}')
            jsonError("key/value not closed");
    }
    if (str[i - 1] != '}')
        jsonError("closing brackets");


}

void    Config::jsonError(std::string msg) {
    std::string ret = "json format error: ";
    ret.append(msg);
    std::cout << ret.c_str() << std::endl;
    exit(EXIT_FAILURE);
}

/* DESTRUCTOR */
Config::~Config(void)
{
//    if (ifs.is_open())
//        ifs.close();
}
