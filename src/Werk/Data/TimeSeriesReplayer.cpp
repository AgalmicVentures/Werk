
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

#include "TimeSeriesReplayer.hpp"

#include "Werk/Data/TimeSeries.hpp"
#include "Werk/Logging/Log.hpp"

namespace werk
{

void TimeSeriesReplayer::addDataSource(TimeSeries *dataSource)
{
	const uint64_t time = dataSource->time();
	const bool finished = !dataSource->moveNext();
	const uint64_t nextTime = dataSource->time();

	if (finished || 0 == nextTime) {
		if (nullptr != _log) {
			_log->log(LogLevel::JSON, "{\"type\":\"timeSeriesReplayer.finishedSource\",\"name\":\"%s\",\"time\":%" PRIu64 "}",
				dataSource->name().c_str(), time);
		}
		return;
	} else if (nextTime < time) {
		if (nullptr != _log) {
			_log->log(LogLevel::JSON, "{\"type\":\"timeSeriesReplayer.unorderedSource\",\"name\":\"%s\",\"time\":%" PRIu64 ",\"nexTime\":%" PRIu64 "}",
				dataSource->name().c_str(), time, nextTime);
		}
		return;
	}

	_dataSources.insert(std::make_pair(nextTime, dataSource));
}

void TimeSeriesReplayer::execute()
{
	//Get the next simulated event and update the simulated clock
	auto i = _dataSources.begin();
	if (_dataSources.end() == i) {
		if (nullptr != _log) {
			_log->logRaw(LogLevel::JSON, "{\"type\":\"timeSeriesReplayer.finished\"}");
		}
		_complete.set();
		return;
	}

	//Get the next event
	TimeSeries *dataSource = i->second;
	const uint64_t time = dataSource->time();

	//Check if this would violate the timeout
	const uint64_t lastTime = _simulatedClock->time();
	if (_timeout > 0 && lastTime > 0 && time > lastTime + _timeout) {
		//Timed out
		_currentDataSource = nullptr;
		_simulatedClock->setTime(lastTime + _timeout);
	} else {
		//Didn't time out, so this is the event
		_currentDataSource = dataSource;
		_simulatedClock->setTime(time);

		//Erase and reinsert at the next time
		_dataSources.erase(i);
		addDataSource(dataSource);
	}

	//All state is updated and ready to go
	_mainAction->execute();
}

}
