#include "Config.hpp"

/* CONSTRUCTOR */
Config::Config(void) {}

/* OVERLOAD */

/* FUNCTIONS */
void	Config::setup(std::string const & filename)
{
    if (filename.compare(filename.size() - 4, 4, "json")) {
        std::cerr << "File extension must be json" << std::endl;
        exit(EXIT_FAILURE);
    }

    ifs.open(filename, std::ifstream::in);
    if (!ifs.is_open())
        error("open()");

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    _file = str;
    parse();
}

/* ft_split of char '\n' from file into a vector of strings */
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
            cfg.push_back(tmp);
	}
}

/* ft_strtrim spaces of each strings from the vector */
void Config::trim() {
    ite = cfg.end();
    for (it = cfg.begin(); it != ite; it++) {
        size_t pos;
        while ((pos = it->find(' ')) != std::string::npos)
            it->erase(pos, 1);
    }
}

void Config::debug() {
    for (it = cfg.begin(); it != ite; it++) {
        std::cout << "|" << *it << "|" << std::endl;
    }
}

// a key-value can be :
// - 2 simple strings -> "name":"Joe"
// - 1 key and an array -> "Subjects":["Math","Phy","Chem"]"
// note: a value that doesn't end with a ',' will not enter this condition
// it should enter ONLY if the key-value is at the end of a array/object
void    parseKeyValue(std::string str) {
    std::cout << str << " | " << "key-value" << std::endl;
}

void Config::parse() {

    vectorize();
    trim();
//    debug();

    ite = cfg.end();
    if (*cfg.begin() != "{")
        error("cfg: object: begin");
    for (it = cfg.begin(); it != ite; it++) {
        if (it->front() == '\"' && it->back() == ',')
            parseKeyValue(*it);
        if (it->back() == ':' || it->back() == '{')
            parse
    }


}


/* DESTRUCTOR */
Config::~Config(void)
{
    if (ifs.is_open())
        ifs.close();
}
