
#pragma once

#include "Werk/Data/Csv/CsvParser.hpp"
#include "Werk/Data/TimeSeries.hpp"

namespace werk
{

class CsvParser;

/**
 * An implementation of a `TimeSeries` for CSV files.
 */
class CsvTimeSeries : public TimeSeries
{
public:

	CsvTimeSeries(CsvParser &parser, size_t timeColumn) :
		_parser(parser), _timeColumn(timeColumn) { }

	virtual uint64_t time() override { return _time; }
	virtual bool moveNext() override {
		_time = 0;
		if (!_parser.moveNext()) {
			return false;
		}

		if (_parser.values().size() <= _timeColumn) {
			return false;
		}

		_time = std::stoull(_parser.values()[_timeColumn]);
		return true;
	}

	const std::vector<std::string> values() const { return _parser.values(); }

protected:
	CsvParser &_parser;
	size_t _timeColumn;
	uint64_t _time = 0;
};

}
