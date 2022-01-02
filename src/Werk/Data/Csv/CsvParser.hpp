
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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
	CHECKED const std::vector<std::string> &headers() const { return _headers; }
	CHECKED const std::vector<std::string> &values() const {
		return _valuesN % 2 == 0 ? _values0 : _values1;
	}
	CHECKED const std::vector<std::string> &lastValues() const {
		return _valuesN % 2 == 0 ? _values1 : _values0;
	}

	CHECKED bool open(const std::string &path);
	CHECKED bool moveNext(bool loadHeaders=false);
	void close();

protected:
	std::ifstream _file;
	std::string _line;
	std::vector<std::string> _headers;
	std::vector<std::string> _values0;
	std::vector<std::string> _values1;
	uint64_t _valuesN = 0;
};

}
