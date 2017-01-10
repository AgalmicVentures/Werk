
#pragma once

#include <string>

namespace werk
{

/**
 * Represents a task to be run on a background thread in a cooperative fashion
 * with other background tasks.
 */
class BackgroundTask
{
public:

	BackgroundTask(const std::string &name) : _name(name) { }

	const std::string &taskName() const { return _name; }

	virtual void executeTask() = 0;

private:
	std::string _name;
};

}
