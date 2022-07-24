
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

#include <cassert>

#include "Werk/Data/Csv/CsvParser.hpp"
#include "Werk/Data/TimeSeries.hpp"
#include "Werk/Utility/Attributes.hpp"

namespace werk
{

class CsvParser;

/**
 * An implementation of a `TimeSeries` for CSV files.
 */
class CsvTimeSeries : public TimeSeries
{
public:
	CsvTimeSeries(const std::string &name, CsvParser &parser, size_t timeColumn=0, int64_t scale=1, int64_t offset=0) :
		TimeSeries(name), _parser(parser), _timeColumn(timeColumn), _scale(scale), _offset(offset) {
		assert(scale > 0);
	}
	virtual ~CsvTimeSeries() { }

	CHECKED virtual uint64_t time() const override { return _time; }
	CHECKED virtual bool moveNext() override {
		_time = 0;
		if (!_parser.moveNext()) {
			return false;
		}

		if (_parser.values().size() <= _timeColumn) {
			return false;
		}

		_time = static_cast<uint64_t>(std::stoll(_parser.values()[_timeColumn]) * _scale + _offset);
		return true;
	}

	CHECKED const std::vector<std::string> values() const { return _parser.lastValues(); }

protected:
	CsvParser &_parser;
	size_t _timeColumn;
	int64_t _scale;
	int64_t _offset;

	uint64_t _time = 0;
};

}
