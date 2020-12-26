
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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

bool CsvParser::moveNext(bool loadHeaders)
{
	//Walk to the next line
	_line.clear();
	while (std::getline(_file, _line)) {
		//Skip blank lines
		if (_line.empty()) {
			continue;
		}

		//Parse either into the headers or the next buffer slot
		if (!loadHeaders) {
			++_valuesN;
		}
		std::vector<std::string> &currentValues = loadHeaders ?
			_headers :
			(_valuesN % 2 == 0 ? _values0 : _values1);
		currentValues.clear();

		//Split on ,
		std::string::size_type i = 0;
		std::string::size_type nextComma = 0;
		do {
			//Find the next command, accounting for quotes
			bool isQuoted = _line[i] == '"';
			if (isQuoted) {
				std::string::size_type nextQuote = _line.find('"', i + 1);
				nextComma = _line.find(',', nextQuote);
			} else {
				nextComma = _line.find(',', i);
			}

			//Grab the value
			std::string value = _line.substr(i, nextComma - i);
			i = nextComma + 1;

			//Remove whitespace
			boost::trim(value);

			//And quotes
			if (value.length() >= 2 && value[0] == '"' && value[value.length() - 1] == '"') {
				value = value.substr(1, value.length() - 2);
			}

			//Finally add the cleaned data
			currentValues.push_back(value);
		} while (nextComma != std::string::npos);

		//Read a line successfully
		return true;
	}

	//Flip the buffer one last time and preserve only the last set of values
	if (!loadHeaders) {
		++_valuesN;
		std::vector<std::string>  &nextValues = _valuesN % 2 == 0 ? _values0 : _values1;
		nextValues.clear();
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
