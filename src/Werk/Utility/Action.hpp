
#pragma once

namespace werk
{

/**
 * An abstract class that represents an action to be executed one or more
 * times, but not in an ongoing fashion.
 */
class Action
{
public:
	virtual void execute() = 0;
};

}
