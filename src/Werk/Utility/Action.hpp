
#pragma once

#include <string>

namespace werk
{

/**
 * An abstract class that represents an action to be executed one or more
 * times, but not in an ongoing fashion (should not execute indefinitely).
 *
 * Named to enable easier debugging.
 */
class Action
{
public:
	Action(const std::string &name) : _name(name) { }

	const std::string &name() const { return _name; }

	virtual void execute() = 0;

private:
	std::string _name;
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
