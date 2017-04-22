
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

	//Get the next event
	TimeSeries *dataSource = i->second;
	uint64_t time = dataSource->time();

	//Check if this would violate the timeout
	uint64_t lastTime = _simulatedClock->time();
	if (_timeout > 0 && lastTime > 0 && time > lastTime + _timeout) {
		//Timed out
		_simulatedClock->setTime(lastTime + _timeout);
	} else {
		//Didn't time out, so this is the event
		_simulatedClock->setTime(time);

		//Erase and reinsert at the next time
		_dataSources.erase(i);
		addDataSource(dataSource);
	}

	//All state is updated and ready to go
	_mainAction->execute();
}

}
