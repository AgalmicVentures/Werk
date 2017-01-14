
#pragma once

#include <string>

namespace werk
{

/**
 * An abstract class that represents an action to be executed one or more
 * times, but not in an ongoing fashion (should not execute indefinitely).
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

}
