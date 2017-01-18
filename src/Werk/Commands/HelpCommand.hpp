
#pragma once

#include "Werk/Logging/Log.hpp"

#include "Command.hpp"

namespace werk
{

class CommandManager;

/**
 * Command that outputs help for all commands.
 */
class HelpCommand : public Command
{
public:

	HelpCommand(Log *log, CommandManager *commandManager) :
		Command("Logs help for all commands."),
		_log(log), _commandManager(commandManager) { }

	bool execute(const std::vector<std::string> &arguments) override;

private:
	Log *_log;
	CommandManager *_commandManager;
};

}
