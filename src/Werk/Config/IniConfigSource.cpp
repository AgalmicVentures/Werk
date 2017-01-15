
#include "IniConfigSource.hpp"

#include <fstream>

namespace werk
{

bool IniConfigSource::reloadConfig(std::map<std::string, std::string> &values) {
	//TODO: exception handling to return false if the file is missing
	std::ifstream infile(_path);

	//Walk the lines
	std::string line;
	while (std::getline(infile, line)) {
		//Trim beginning of line whitespace
		std::size_t firstNonWhitespacePos = line.find_first_not_of(' ');
		std::size_t firstNonWhitespace = firstNonWhitespacePos == std::string::npos ? line.length() : firstNonWhitespacePos;
		line = line.substr(firstNonWhitespace);

		//Skip blank lines and comments which start with #
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
		values[line.substr(0, equalPos)] = line.substr(equalPos + 1);
	}

	return true;
}

}
