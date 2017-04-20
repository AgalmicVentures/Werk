
#include "TimeSeriesReplayer.hpp"

#include "Werk/Data/TimeSeries.hpp"
#include "Werk/Logging/Log.hpp"

namespace werk
{

void TimeSeriesReplayer::addDataSource(TimeSeries *dataSource)
{
	uint64_t time = dataSource->time();
	bool finished = !dataSource->moveNext();
	uint64_t nextTime = dataSource->time();

	if (finished || 0 == nextTime) {
		if (nullptr != _log) {
			//TODO: would be nice if this had the path
			_log->log(LogLevel::JSON, "{\"type\":\"timeSeriesReplayer.finishedSource\",\"at\":%" PRIu64 "}", time);
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

	//Get the time and update the simulated clock
	TimeSeries *dataSource = i->second;
	uint64_t time = dataSource->time();
	_simulatedClock->setTime(time);

	//Erase and reinsert at the next time
	_dataSources.erase(i);
	addDataSource(dataSource);

	//Run the main action
	_mainAction->execute();
}

}
