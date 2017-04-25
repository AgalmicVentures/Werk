
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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
