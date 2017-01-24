
#pragma once

#include <map>
#include <string>
#include <vector>

#include "Werk/Logging/Log.hpp"
#include "Werk/Logging/Loggable.hpp"

#include "Command.hpp"
#include "EchoCommand.hpp"

namespace werk
{

/**
 * Holds a collection of named commands and assists in forwarding user input to them.
 */
class CommandManager : public Loggable
{
public:

	CommandManager(Log *log, bool defaultCommands=true) : _log(log) {
		//Default commands
		if (defaultCommands) {
			_commands["null"] = new NullCommand();
			_commands["?"] = _commands["help"] = new ActionCommand(
				new LogAction("LogHelp", this, _log),
				"Logs command help.");

			_commands["echo"] = new EchoCommand(log);
			_commands["error"] = new EchoCommand(log, LogLevel::ERROR);
			_commands["warning"] = new EchoCommand(log, LogLevel::WARNING);
		}
	}

	const std::map<std::string, Command *> &commands() const { return _commands; }

	void add(const std::string &name, Command *command) {
		_commands[name] = command;
	}

	bool execute(const std::string &commandLine);
	bool execute(const std::vector<std::string> &arguments);

	//Helper to create a new CommandAction from a command line
	CommandAction *newCommandAction(const std::string &name, const std::string &commandLine);

	void logTo(Log *log) const override;

private:
	Log *_log;

	std::map<std::string, Command *> _commands;
};

}
