
#include "CsvParser.hpp"

#include <boost/algorithm/string.hpp>

namespace werk
{

bool CsvParser::open(const std::string &path)
{
	if (_file.is_open()) {
		return false;
	}

	_file.open(path);
	return _file.is_open();
}

bool CsvParser::moveNext()
{
	//Walk to the next line
	_line.clear();
	while (std::getline(_file, _line)) {
		//Skip blank lines
		if (_line.empty()) {
			continue;
		}

		//Split on ,
		//TODO: handle quotes properly
		boost::split(_values, _line, boost::is_any_of(","));
		for (auto &value : _values) {
			boost::trim(value);
		}

		//Read a line successfully
		return true;
	}

	return false;
}

void CsvParser::close()
{
	if (_file.is_open()) {
		_file.close();
	}
}

}
