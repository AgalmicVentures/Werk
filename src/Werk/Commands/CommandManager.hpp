
#pragma once

#include <map>
#include <string>
#include <vector>

#include "Werk/Commands/Command.hpp"
#include "Werk/Commands/EchoCommand.hpp"
#include "Werk/Logging/Log.hpp"

namespace werk
{

class CommandManager
{
public:

	CommandManager(Log *log, bool defaultCommands=true) : _log(log) {
		//Default commands
		if (defaultCommands) {
			_commands["echo"] = new EchoCommand(log);
			_commands["error"] = new EchoCommand(log, LogLevel::ERROR);
			_commands["null"] = new NullCommand();
			_commands["warning"] = new EchoCommand(log, LogLevel::WARNING);
		}
	}

	std::map<std::string, Command *> &commands() { return _commands; }
	const std::map<std::string, Command *> &commands() const { return _commands; }

	bool execute(const std::string &commandLine);
	bool execute(const std::vector<std::string> &arguments);

	//Helper to create a new CommandAction from a command line
	CommandAction *newCommandAction(const std::string &commandLine);

private:
	Log *_log;

	std::map<std::string, Command *> _commands;
};

}
