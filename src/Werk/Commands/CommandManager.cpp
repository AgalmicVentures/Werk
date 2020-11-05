
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "CommandManager.hpp"

#include <boost/algorithm/string.hpp>

#include "Command.hpp"

namespace werk
{

bool CommandManager::execute(const std::string &commandLine)
{
	//History is only added here, from human executions. This enables neat stuff like the redo
	//command and allows the command system to still be used in an automated fashion.
	_commandHistory.emplace_back(_clock.time(), commandLine);

	//Parse the arguments, ignoring extra whitespace
	std::string trimmedCommandLine = boost::trim_copy(commandLine);
	std::vector<std::string> arguments;
	boost::split(arguments, trimmedCommandLine, boost::is_any_of(" \t"), boost::token_compress_on);
	return arguments[0] == "" ? false : execute(arguments);
}

bool CommandManager::execute(const std::vector<std::string> &arguments)
{
	if (arguments.empty()) {
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

CommandAction *CommandManager::newCommandAction(const std::string &name, const std::string &commandLine)
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
	CommandAction *commandAction = new CommandAction(name, command);
	commandAction->arguments() = arguments;
	return commandAction;
}

void CommandManager::logTo(Log *log) const
{
	log->log(LogLevel::INFO, "<CommandManager> Commands (%zu):", _commands.size());
	for (const auto &i : _commands) {
		_log->log(LogLevel::INFO, "  %16s   %s", i.first.c_str(), i.second->help().c_str());
	}
}

}
