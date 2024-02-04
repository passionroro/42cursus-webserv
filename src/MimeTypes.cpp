#include "MimeTypes.hpp"

MimeTypes::~MimeTypes(void)
{
}

MimeTypes::MimeTypes(void)
{
	std::ifstream	in("config/mime.types");
	std::string		line;

	if (!in.is_open())
		return ;
	while (std::getline(in, line))
	{
		std::stringstream	split(line);
		std::string key;
		std::string value;

		split >> value;
		split >> key;
		key.pop_back();
		_map.insert(std::make_pair(key, value));
	}
	in.close();
}

MapStr&	MimeTypes::getMap(void)
{
	return _map;
}
