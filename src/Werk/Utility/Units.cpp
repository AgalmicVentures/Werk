
/*
 * Copyright (c) 2015-2019 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Units.hpp"

namespace werk
{

const std::string NO_UNIT("");

const UnitsT STORAGE_UNITS = {
	{ "P", 1024ul * 1024 * 1024 * 1024 * 1024 },
	{ "T", 1024ul * 1024 * 1024 * 1024 },
	{ "G", 1024ul * 1024 * 1024 },
	{ "M", 1024ul * 1024 },
	{ "K", 1024ul },
};
const UnitsT TIME_UNITS = {
	{ "d", 24ul * 60 * 60 * 1000 * 1000 * 1000 },
	{ "h", 60ul * 60 * 1000 * 1000 * 1000 },
	{ "m", 60ul * 1000 * 1000 * 1000 },
	{ "s", 1000ul * 1000 * 1000 },
	{ "ms", 1000ul * 1000 },
	{ "us", 1000ul },
	{ "ns", 1ul },
};

void formatUnits(uint64_t value, const UnitsT &units, double &scaledValue, const char *&unitStr)
{
	//By default, have no units
	scaledValue = static_cast<double>(value);
	unitStr = NO_UNIT.c_str();

	//Otherwise, find an appropriate scaling factor
	for (const auto &unit : units) {
		if (value >= unit.second) {
			scaledValue /= static_cast<double>(unit.second);
			unitStr = unit.first.c_str();
			break;
		}
	}
}

uint64_t parseUnits(const std::string &value, const UnitsT &units)
{
	//Parse out the number
	size_t n = 0;
	bool hadDecimal = false;
	while (n < value.length()) {
		char ch = value[n];
		bool isDecimal = ch == '.';

		//Digits are ok, the negative sign can be the first thing, and there can be a decimal point
		if (('0' <= ch && ch <= '9') || (n == 0 && ch == '-') || (!hadDecimal && isDecimal)) {
			n += 1;
			if (isDecimal) {
				hadDecimal = true;
			}
		} else {
			break;
		}
	}

	//Were there no units?
	if (n == value.length()) {
		return std::stoull(value);
	}

	//Find the units, default multiplier of 1
	uint64_t multiplier = 1;
	const std::string unitStr = value.substr(n);
	for (const auto &unit : units) {
		if (unit.first == unitStr) {
			multiplier = unit.second;
			break;
		}
	}

	//Load it as a double so decimals will work
	const double baseValue = std::stod(value.substr(0, n));
	return static_cast<uint64_t>(baseValue * static_cast<double>(multiplier));
}

}
