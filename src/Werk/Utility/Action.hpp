
#pragma once

#include <string>
#include <vector>

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
	virtual ~Action() { }

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
//NOTE: Although this shared instance is not const, that's not actually a problem
extern NullAction NULL_ACTION;

/**
 * An action that increments a counter every time it is executed. Very useful
 * for testing, and appropriate where an entirely separate `Counter` would be
 * inconvenient.
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

/**
 * An action that executes a pair of other actions.
 */
class CompoundAction : public Action
{
public:
	CompoundAction(const std::string &name) : Action(name) { }

	std::vector<Action *> &actions() { return _actions; }
	const std::vector<Action *> &actions() const { return _actions; }

	void execute() override {
		for (Action *action : _actions) {
			action->execute();
		}
	}

private:
	std::vector<Action *> _actions;
};

/**
 * An action that executes a pair of other actions.
 */
class PairAction : public Action
{
public:
	PairAction(const std::string &name, Action *action1, Action *action2) :
		Action(name), _action1(action1), _action2(action2) { }

	void execute() override {
		_action1->execute();
		_action2->execute();
	}

private:
	Action *_action1;
	Action *_action2;
};

/**
 * An action that resets a component (many components have a `void reset()` method).
 */
template <typename T>
class ResetAction : public Action
{
public:
	ResetAction(const std::string &name, T &object) :
		Action(name), _object(object) { }

	void execute() override { _object.reset(); }

private:
	T &_object;
};

}
