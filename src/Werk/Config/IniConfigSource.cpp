
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

#include "IniConfigSource.hpp"

#include <fstream>

namespace werk
{

bool IniConfigSource::reloadConfig(std::map<std::string, std::string> &values) {
	//Open the file
	std::ifstream infile(_path);
	if (infile.fail()) {
		return false;
	}

	//Walk the lines
	std::string line;
	while (std::getline(infile, line)) {
		//Trim beginning of line whitespace
		const std::size_t firstNonWhitespacePos = line.find_first_not_of(' ');
		const std::size_t firstNonWhitespace = firstNonWhitespacePos == std::string::npos ? line.length() : firstNonWhitespacePos;
		line = line.substr(firstNonWhitespace);

		//Skip blank lines and comments which start with #
		if (line.empty() || line[0] == '#') {
			continue;
		}

		//Find the =
		const std::size_t equalPos = line.find('=');
		if (equalPos == std::string::npos) {
			//No value found, error
			return false;
		}

		//Copy the key and value into the map
		values[line.substr(0, equalPos)] = line.substr(equalPos + 1);
	}

	return true;
}

}
