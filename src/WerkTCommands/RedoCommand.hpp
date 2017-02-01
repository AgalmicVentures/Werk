
#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Logging/Log.hpp"

#include "Command.hpp"

namespace werk
{

class CommandManager;

/**
 * Command that re-runs the last command.
 */
class RedoCommand : public Command
{
public:

	RedoCommand(CommandManager &commandManager) :
		Command("Repeats the last command."),
		_commandManager(commandManager) { }

	bool execute(const std::vector<std::string> &arguments) override;

private:
	CommandManager &_commandManager;
};

}
