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

void    parseChar(char c, std::string tmp, std::string _file) {

}

void Config::parse() {

    char parse[7] = {' ', '{', '}', '[', ']', ',', '\n'};
    std::string tmp = _file;
    for (int i = 0; i < 7; i++) {
        int start;
        int end = 0;
        while (end != -1) {
            start = end + 1;
            if (end == 0)
                start = 0;
            end = _file.find(parse[i], start);
            tmp.append(_file.substr(start, end - start));
        }
    }


//    std::vector<std::string>    cfg;
//    std::vector<std::string>::iterator it;
//    std::vector<std::string>::iterator ite = cfg.end();
//    for (it = cfg.begin(); it != ite; ++it) {
//        std::cout << "|" << *it << "|" << std::endl;
//    }
}

/* DESTRUCTOR */
Config::~Config(void)
{
    if (ifs.is_open())
        ifs.close();
}
