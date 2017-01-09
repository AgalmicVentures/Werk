
#include "MapConfig.hpp"

#include <fstream>

namespace werk
{

bool MapConfig::loadFromFile(const char *filename) {
	std::ifstream infile(filename);

	std::string line;
	while (std::getline(infile, line)) {
		//Trim beginning of line whitespace
		std::size_t firstNonWhitespacePos = line.find_first_not_of(' ');
		std::size_t firstNonWhitespace = firstNonWhitespacePos == std::string::npos ? line.length() : firstNonWhitespacePos;
		line = line.substr(firstNonWhitespace);

		//Skip blank lines and comments
		if (line.length() == 0 || line[0] == '#') {
			continue;
		}

		//Find the =
		std::size_t equalPos = line.find('=');
		if (equalPos == std::string::npos) {
			//TODO: output error? return false?
			continue;
		}

		//Copy the key and value into the map
		_values[line.substr(0, equalPos)] = line.substr(equalPos + 1);
	}

	return true;
}

}
