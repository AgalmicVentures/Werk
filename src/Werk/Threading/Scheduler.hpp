
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
		uint64_t time = _clock->time();
		while (_tasks.size() > 0) {
			//Get the next task
			auto i = _tasks.begin();
			uint64_t nextTime = i->first;
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
