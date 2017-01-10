
#pragma once

#include <map>
#include <string>
#include <vector>

#include "Werk/Commands/Command.hpp"
#include "Werk/Commands/EchoCommand.hpp"
#include "Werk/Logging/Logger.hpp"

namespace werk
{

class CommandManager
{
public:

	CommandManager(Logger *log) : _log(log) {
		//Default commands
		_commands["echo"] = new EchoCommand(log);
		_commands["error"] = new EchoCommand(log, LogLevel::ERROR);
		_commands["null"] = new NullCommand();
		_commands["warning"] = new EchoCommand(log, LogLevel::WARNING);
	}

	std::map<std::string, Command *> &commands() { return _commands; }
	const std::map<std::string, Command *> &commands() const { return _commands; }

	bool execute(const std::string &commandLine);
	bool execute(const std::vector<std::string> &arguments);

private:
	Logger *_log;

	std::map<std::string, Command *> _commands;
};

}
