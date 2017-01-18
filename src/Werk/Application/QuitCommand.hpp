
#pragma once

#include "Werk/Commands/Command.hpp"
#include "Werk/Logging/Log.hpp"

namespace werk
{

class ApplicationContext;

/**
 * Command that shuts the program down cleanly.
 */
class QuitCommand : public Command
{
public:

	QuitCommand(ApplicationContext *applicationContext) :
		Command("Quits the program cleanly."),
		_applicationContext(applicationContext) { }

	bool execute(const std::vector<std::string> &arguments) override;

private:
	ApplicationContext *_applicationContext;
};

}
