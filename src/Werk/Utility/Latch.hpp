
#pragma once

#include "Action.hpp"

namespace werk
{

/**
 * A set-reset latch.
 *
 * Templated on type so that the boolean can be made volatile if necessary.
 */
template <typename T=bool>
class Latch
{
public:
	Latch(bool value=false) : _value(value) { }

	bool value() const { return _value; }
	void set() { _value = true; }
	void reset() { _value = false; }

private:
	T _value;
};

/**
 * An action that resets a `Latch`.
 */
template <typename T=bool>
class ResetLatchAction : public Action
{
public:
	ResetLatchAction(const std::string &name, Latch<T> &latch) :
		Action(name), _latch(latch) { }

	void execute() override { _latch.reset(); }

private:
	Latch<T> &_latch;
};

/**
 * An action that sets a `Latch`.
 */
template <typename T=bool>
class SetLatchAction : public Action
{
public:
	SetLatchAction(const std::string &name, Latch<T> &latch) :
		Action(name), _latch(latch) { }

	void execute() override { _latch.set(); }

private:
	Latch<T> &_latch;
};

/**
 * An action that executes another action, if the latch is set.
 */
template <typename T=bool>
class ConditionalAction : public Action
{
public:
	ConditionalAction(const std::string &name, Latch<T> &latch, Action *action) :
		Action(name), _latch(latch), _action(action) { }

	void execute() override {
		if (_latch.value()) {
			_action->execute();
		}
	}

private:
	Latch<T> &_latch;
	Action *_action;
};

}
