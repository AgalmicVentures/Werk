
#include "CommandManager.hpp"

#include <boost/algorithm/string.hpp>

#include "Command.hpp"

namespace werk
{

bool CommandManager::execute(const std::string &commandLine)
{
	std::vector<std::string> arguments;
	boost::split(arguments, commandLine, boost::is_any_of(" \t"));
	return execute(arguments);
}

bool CommandManager::execute(const std::vector<std::string> &arguments)
{
	if (arguments.size() == 0) {
		_log->logRaw(LogLevel::CRITICAL, "Got zero-length arguments to CommandManager::execute");
		return false;
	}

	const std::string &commandName = arguments[0];
	auto commandIter = _commands.find(commandName);
	if (commandIter == _commands.end()) {
		_log->log(LogLevel::ERROR, "Command not found: %s", commandName.c_str());
		return false;
	}

	Command *command = commandIter->second;
	return command->execute(arguments);
}

CommandAction *CommandManager::newCommandAction(const std::string &commandLine)
{
	std::vector<std::string> arguments;
	boost::split(arguments, commandLine, boost::is_any_of(" \t"));
	const std::string &commandName = arguments[0];
	auto commandIter = _commands.find(commandName);
	if (commandIter == _commands.end()) {
		_log->log(LogLevel::ERROR, "Command not found for action: %s", commandName.c_str());
		return nullptr;
	}

	Command *command = commandIter->second;
	CommandAction *commandAction = new CommandAction(command);
	commandAction->arguments() = arguments;
	return commandAction;
}

}
