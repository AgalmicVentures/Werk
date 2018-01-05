
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

namespace werk
{

/**
 * A task for the scheduler, including information about when to execute it
 * and when to repeat it.
 */
class ScheduledTask
{
public:
	ScheduledTask(Action *action, uint64_t interval, int64_t count=-1) :
		_action(action), _interval(interval), _count(count) { }

	uint64_t interval() const { return _interval; }
	int64_t count() const { return _count; }
	bool repeat() const { return _count != 0; }

	void execute() {
		_action->execute();
		if (_count > 0) {
			_count -= 1;
		}
	}

protected:
	Action *_action;
	uint64_t _interval;
	int64_t _count;
};

class Scheduler : public Action
{
public:
	Scheduler(const std::string &name, const Clock *clock) :
		Action(name), _clock(clock) { }

	void schedule(Action *action, uint64_t time, uint64_t interval, int64_t repeat=-1) {
		//TODO: check time versus now?
		ScheduledTask *task = new ScheduledTask(action, interval, repeat);
		_tasks.emplace(time, task);
	}

	void execute() override {
		const uint64_t time = _clock->time();
		for (auto i = _tasks.begin(); i != _tasks.end(); i = _tasks.begin()) {
			//Get the next task
			const uint64_t nextTime = i->first;
			ScheduledTask *task = i->second;

			//Is it ready?
			if (nextTime > time) {
				break;
			}

			//Run the task, then move it forward in time
			task->execute();
			_tasks.erase(i);
			if (task->repeat()) {
				_tasks.emplace(nextTime + task->interval(), task);
			} else {
				delete task;
			}
		}
	}

protected:
	const Clock *_clock;

	//TODO: do these in place rather than pointers?
	//Scheduled tasks by their next execution time
	std::multimap<uint64_t, ScheduledTask *> _tasks;
};

}
