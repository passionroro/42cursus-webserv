#include "MimeTypes.hpp"

MimeTypes::~MimeTypes(void)
{
}

MimeTypes::MimeTypes(void)
{
	std::ifstream	in("mime.types");
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
		//std::cout << key << value << std::endl;
		_map.insert(std::make_pair(key, value));
	}
	in.close();
}

/*int	main(void)
{
	MimeTypes	test;

	return (0);
}*/
