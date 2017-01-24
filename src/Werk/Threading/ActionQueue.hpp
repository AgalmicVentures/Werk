
#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <queue>

#include "Werk/Utility/Action.hpp"

namespace werk
{

/**
 * An queue for one time `Action`s, templated to allow use with the same or
 * different threads.
 */
template <typename Q=boost::lockfree::spsc_queue<Action *, boost::lockfree::capacity<1024> > >
class ActionQueue : public Action
{
public:
	ActionQueue(const std::string &name) : Action(name) { }

	void push(Action *action) { _queue.push(action); }

	void execute() override {
		Action *action;
		while (_queue.pop(action)) {
			action->execute();
		}
	}

private:
	Q _queue;
};

}
