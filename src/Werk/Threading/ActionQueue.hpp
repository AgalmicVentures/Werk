
/*
 * Copyright (c) 2015-2019 Agalmic Ventures LLC (www.agalmicventures.com)
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
