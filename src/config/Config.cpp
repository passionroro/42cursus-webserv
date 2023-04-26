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

    _data = parse();

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
    while ((pos = str.find('\r')) != std::string::npos)
        str.erase(pos, 1);
    while ((pos = str.find('\t')) != std::string::npos)
        str.erase(pos, 1);
}

/*void Config::debug() {
    std::cout << "|" << str << "|" << std::endl;
}*/

Object Config::parse() {

    vectorize();
    trim();
//    debug();

    int i = 0;
    if (str[i] != '{')
        jsonError("opening brackets");

    Value tmp;
    Object server;
    server = tmp.valueIsObject(str, &i);
    return server;
}

void    Config::jsonError(std::string msg) {
    std::string ret = "json format error: ";
    ret.append(msg);
    std::cout << ret.c_str() << std::endl;
    exit(EXIT_FAILURE);
}

Object const&	Config::getData(void) const
{
	return (_data);
}

/* DESTRUCTOR */
Config::~Config(void)
{
//    if (ifs.is_open())
//        ifs.close();
}
