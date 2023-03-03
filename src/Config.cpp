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

/* An object is an unordered set of name/value pairs.
 * An object begins with { and ends with }.
 * Each name is followed by a colon and the name/value pairs are separated by a comma.*/
//Object Config::valueIsObject(std::string str, int *i) {
//    (void)str, i;
//}

/* A value can be a string in double quotes, or a number, or true or false or null, or an object or an array.
 * These structures can be nested.*/


void Config::parse() {

    vectorize();
    trim();

    int i = 0;
    if (str[i] != '{')
        jsonError("opening brackets");

//    Object main;
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
