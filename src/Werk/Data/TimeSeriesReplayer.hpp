
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

#pragma once

#include <map>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class Log;
class TimeSeries;

//TODO: is directly passing the latch the best way to do this?
class TimeSeriesReplayer : public Action
{
public:
	TimeSeriesReplayer(const std::string &name, Clock *simulatedClock, Latch<volatile bool> &complete,
		Action *mainAction, Log *log=nullptr, uint64_t timeout=0) :
		Action(name), _simulatedClock(simulatedClock), _complete(complete),
		_mainAction(mainAction), _log(log), _timeout(timeout) { }

	const TimeSeries *currentDataSource() const { return _currentDataSource; }

	void addDataSource(TimeSeries *dataSource);

	void execute() override;

protected:
	Clock *_simulatedClock;
	Latch<volatile bool> &_complete;
	Action *_mainAction;
	Log *_log;
	uint64_t _timeout;

	std::multimap<uint64_t, TimeSeries *> _dataSources;
	TimeSeries *_currentDataSource = nullptr;
};

}
