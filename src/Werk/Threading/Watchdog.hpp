
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <cassert>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Attributes.hpp"
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
	virtual ~Watchdog() { }

	CHECKED uint64_t interval() const { return _interval; }
	CHECKED uint64_t allowedMisses() const { return _allowedMisses; }
	CHECKED Action *action() const { return _action; }

	CHECKED bool latch() const { return _latch.value(); }
	void reset() { _latch.reset(); }

	void execute() override {
		const uint64_t time = _clock->time();
		if (_lastTime == 0) {
			_lastTime = time;
			return;
		}

		//If the flag is set, everything is fine
		if (!_latch.value()) {
			_latch.set();
			_lastTime = time;
			_misses = 0;
			return;
		}

		//Otherwise, has the interval passed?
		if (time > _lastTime + _interval * (_misses + 1)) {
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
	Action * const _action;

	uint64_t _lastTime = 0;
	uint64_t _misses = 0;

	Latch<std::atomic<bool> > _latch;
};

}
