
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
#include "Werk/Utility/Latch.hpp"

namespace werk
{

/**
 * A timer that executes an action regularly up to a certain number of times (or -1 for indefinitely).
 */
class Timer : public Action
{
public:
	Timer(const std::string &name, const Clock *clock, Action *action, uint64_t interval, int64_t count=-1) :
		Action(name), _clock(clock), _interval(interval), _action(action), _count(count) {
		assert(nullptr != action);
	}

	uint64_t interval() const { return _interval; }
	int64_t count() const { return _count; }
	void setCount(int64_t count) { _count = count; }
	Action *action() { return _action; }

	void execute() override {
		if (0 == _count) {
			return;
		}

		const uint64_t time = _clock->time();
		if (_lastTime == 0) {
			//On the first time given, start waiting
			_lastTime = time;
		} else if (_lastTime + _interval <= time) {
			//Then wait until the elapsed time is greater than the interval
			_action->execute();
			_lastTime = time;
			if (_count > 0) {
				_count -= 1;
			}
		}
	}

private:
	const Clock *_clock;
	uint64_t _interval;
	Action *_action;

	uint64_t _lastTime = 0;
	int64_t _count = 0;
};

}
