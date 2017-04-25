
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <cstdint>
#include <limits>

#include "Action.hpp"

namespace werk
{

/**
 * Various modes for the `Counter` class.
 */
enum class CounterMode
{
	ADD,
	MULTIPLY,
	LSHIFT,
};

/**
 * A integer counter that encapsulates most common behaviors. Similar to the
 * `IdGenerator` class, except with static parameters.
 *
 * NOTE: Not compatible with doubles due to limitations on template parameters.
 */
template <
	typename T=uint64_t,
	T initialValue=0,
	T incrementValue=1,
	CounterMode mode=CounterMode::ADD
>
class Counter
{
public:

	T value() const { return _value; }

	void reset() { _value = initialValue; }
	void increment() {
		switch (mode) {
		case CounterMode::ADD:
			_value += incrementValue;
			break;
		case CounterMode::MULTIPLY:
			_value *= incrementValue;
			break;
		case CounterMode::LSHIFT:
			_value <<= incrementValue;
			break;
		}
	}

private:
	T _value = initialValue;
};

/**
 * An action that increments a `Counter`. To reset, use a generic `ResetAction`.
 */
template <
	typename T=uint64_t,
	T initialValue=0,
	T incrementValue=1,
	CounterMode mode=CounterMode::ADD
>
class IncrementCounterAction : public Action
{
public:
	IncrementCounterAction(const std::string &name, Counter<T, initialValue, incrementValue, mode> &counter) :
		Action(name), _counter(counter) { }

	void execute() override { _counter.increment(); }

private:
	Counter<T, initialValue, incrementValue, mode> &_counter;
};

}
