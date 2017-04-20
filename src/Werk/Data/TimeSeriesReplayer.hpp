
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
	TimeSeriesReplayer(const std::string &name, Clock *simulatedClock, Latch<volatile bool> &complete, Action *mainAction, Log *log=nullptr) :
		Action(name), _simulatedClock(simulatedClock), _complete(complete), _mainAction(mainAction), _log(log) { }

	void addDataSource(TimeSeries *dataSource);

	void execute() override;

protected:
	Clock *_simulatedClock;
	Latch<volatile bool> &_complete;
	Action *_mainAction;
	Log *_log;

	std::multimap<uint64_t, TimeSeries *> _dataSources;
};

}
