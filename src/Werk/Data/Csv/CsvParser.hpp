
#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

class CsvParser
{
public:

	CHECKED bool isOpen() const { return _file.is_open(); }
	CHECKED const std::string &line() const { return _line; }
	CHECKED const std::vector<std::string> &values() const { return _values; }

	CHECKED bool open(const std::string &path);
	CHECKED bool moveNext();
	void close();

protected:
	std::ifstream _file;
	std::string _line;
	std::vector<std::string> _values;
};

}
