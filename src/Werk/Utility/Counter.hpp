
#pragma once

#include <cstdint>
#include <limits>

#include "Action.hpp"

namespace werk
{

//TODO: consider the relationship between this and the IdGenerator class -- do they both need to exist?
//TODO: reset, increment actions (is there a clean way to pass these template parameters around in C++11?

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
 * A integer counter that encapsulates most common behaviors.
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
 * An action that resets a `Counter`.
 */
template <
	typename T=uint64_t,
	T initialValue=0,
	T incrementValue=1,
	CounterMode mode=CounterMode::ADD
>
class ResetCounterAction : public Action
{
public:
	ResetCounterAction(const std::string &name, Counter<T, initialValue, incrementValue, mode> &counter) :
		Action(name), _counter(counter) { }

	void execute() override { _counter.reset(); }

private:
	Counter<T, initialValue, incrementValue, mode> &_counter;
};

/**
 * An action that increments a `Counter`.
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
