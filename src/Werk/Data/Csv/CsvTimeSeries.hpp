
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

	CsvTimeSeries(CsvParser &parser, size_t timeColumn=0, int64_t scale=1, int64_t offset=0) :
		_parser(parser), _timeColumn(timeColumn), _scale(scale), _offset(offset) {
		assert(scale > 0);
	}

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

	CHECKED const std::vector<std::string> values() const { return _parser.values(); }

protected:
	CsvParser &_parser;
	size_t _timeColumn;
	int64_t _scale;
	int64_t _offset;

	uint64_t _time = 0;
};

}
