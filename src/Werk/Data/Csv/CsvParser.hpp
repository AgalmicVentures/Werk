
#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace werk
{

class CsvParser
{
public:

	bool isOpen() const { return _file.is_open(); }
	const std::string &line() const { return _line; }
	const std::vector<std::string> &values() const { return _values; }

	bool open(const std::string &path);
	bool moveNext();
	void close();

protected:
	std::ifstream _file;
	std::string _line;
	std::vector<std::string> _values;
};

}
