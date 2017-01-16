
#pragma once

#include <string>

#include "NamedObject.hpp"

namespace werk
{

/**
 * An abstract class that represents an action to be executed one or more
 * times, but not in an ongoing fashion (should not execute indefinitely).
 *
 * Named to enable easier debugging.
 */
class Action : public NamedObject
{
public:
	Action(const std::string &name) : NamedObject(name) { }

	virtual void execute() = 0;
};

/**
 * Do nothing action.
 */
class NullAction : public Action
{
public:
	NullAction(const std::string &name) : Action(name) { }

	void execute() override { }
};

/**
 * An action that sets a flag (which can be reset, like a latch).
 *
 * Templated on type so that the boolean can be made volatile if necessary.
 */
template <typename T=bool>
class LatchAction : public Action
{
public:
	LatchAction(const std::string &name) : Action(name) { }

	bool flag() const { return _flag; }
	void set() { _flag = true; }
	void reset() { _flag = false; }

	void execute() override { set(); }

private:
	T _flag = false;
};

/**
 * An action that increments a counter every time it is executed. Very useful
 * for testing.
 */
template <typename T=uint64_t>
class CounterAction : public Action
{
public:
	CounterAction(const std::string &name) : Action(name) { }

	T count() const { return _count; }
	void reset() const { _count = 0; }

	void execute() override { _count += 1; }

private:
	T _count = 0;
};

}
