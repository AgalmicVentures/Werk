
#include "TimeSeriesReplayer.hpp"

#include "Werk/Data/TimeSeries.hpp"

namespace werk
{

void TimeSeriesReplayer::addDataSource(TimeSeries *dataSource)
{
	if (!dataSource->moveNext()) {
		return;
	}

	uint64_t nextTime = dataSource->time();
	if (0 != nextTime) {
		_dataSources.insert(std::make_pair(nextTime, dataSource));
	}
}

void TimeSeriesReplayer::execute()
{
	//Get the next simulated event and update the simulated clock
	auto i = _dataSources.begin();
	if (_dataSources.end() == i) {
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
