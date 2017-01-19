
#pragma once

#include <cassert>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

/**
 * A watchdog timer that resets a flag to false then checks if it has been set
 * to true at a set interval. If it isn't set for more than an allowed number
 * of misses in row, an action is taken.
 */
class Watchdog : public Action
{
public:
	Watchdog(const std::string &name, const Clock *clock, Action *action, uint64_t interval, uint64_t allowedMisses=0) :
		Action(name), _clock(clock), _interval(interval), _allowedMisses(allowedMisses), _action(action) {
		assert(nullptr != action);
	}

	uint64_t interval() const { return _interval; }
	uint64_t allowedMisses() const { return _allowedMisses; }
	Action *action() { return _action; }

	bool latch() const { return _latch.value(); }
	void reset() { _latch.reset(); }

	void execute() override {
		uint64_t time = _clock->time();

		//If the flag is set, everything is fine
		if (!_latch.value()) {
			_latch.set();
			_lastTime = time;
			_misses = 0;
			return;
		}

		//Otherwise, has the interval passed?
		if (time > _lastTime + _interval) {
			_misses += 1;

			//Only execute the action once
			if (_misses == _allowedMisses + 1) {
				_action->execute();
			}
		}
	}

private:
	const Clock *_clock;
	uint64_t _interval;
	uint64_t _allowedMisses;
	Action *_action;

	uint64_t _lastTime = 0;
	uint64_t _misses = 0;

	Latch<volatile bool> _latch;
};

}
