
#pragma once

#include <map>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class TimeSeries;

//TODO: is directly passing the latch the best way to do this?
class SimulatorAction : public Action
{
public:
	SimulatorAction(const std::string &name, Clock *simulatedClock, Latch<volatile bool> &complete, Action *mainAction) :
		Action(name), _simulatedClock(simulatedClock), _complete(complete), _mainAction(mainAction) { }

	void execute() override;

protected:
	Clock *_simulatedClock;
	Latch<volatile bool> &_complete;
	Action *_mainAction;

	std::multimap<uint64_t, TimeSeries *> _dataSources;
};

}
