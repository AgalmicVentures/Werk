
#include "Simulator.hpp"

#include "Werk/Utility/TimeSeries.hpp"

namespace werk
{

void SimulatorAction::execute()
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
	uint64_t nextTime = dataSource->nextTime();
	_dataSources.erase(i);
	if (0 != nextTime) {
		dataSource->moveNext();
		_dataSources.insert(std::make_pair(nextTime, dataSource));
	}

	//Run the main action
	_mainAction->execute();
}

}
