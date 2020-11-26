
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

#pragma once

#include <cinttypes>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "Werk/Logging/Log.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Version.hpp"

#include "Command.hpp"
#include "EchoCommand.hpp"
#include "RedoCommand.hpp"

namespace werk
{

/**
 * Represents a time a command was run.
 */
class CommandHistory
{
public:

	CommandHistory(uint64_t time, const std::string &commandLine) :
		_time(time), _commandLine(commandLine) { }

	uint64_t time() const { return _time; }
	const std::string &commandLine() const { return _commandLine; }

	void writeJson(FILE *file) const {
		std::fprintf(file, "{\"t\": %" PRIu64 ", \"commandLine\": \"%s\"}\n", _time, _commandLine.c_str());
	}

private:
	uint64_t _time;
	std::string _commandLine;
};

/**
 * Holds a collection of named commands and assists in forwarding user input to them.
 */
class CommandManager : public Loggable
{
public:

	CommandManager(const Clock &clock, Log *log, bool defaultCommands=true) :
		_clock(clock), _log(log)
	{
		//Default commands
		if (defaultCommands) {
			_commands["null"] = new NullCommand();
			_commands["?"] = _commands["help"] = new ActionCommand(
				new LogAction("LogHelp", this, _log),
				"Logs command help.");

			_commands["redo"] = new RedoCommand(*this);

			_commands["echo"] = new EchoCommand(log);
			_commands["error"] = new EchoCommand(log, LogLevel::ERROR);
			_commands["warning"] = new EchoCommand(log, LogLevel::WARNING);

			_commands["version"] = new ActionCommand(
				new LogAction("LogVersion", new StringLoggable(std::string("Version: ") + getVersion()), log),
				"Logs the version of Werk underlying the application.");
		}
	}

	const std::map<std::string, Command *> &commands() const { return _commands; }
	const std::vector<CommandHistory> &commandHistory() const { return _commandHistory; }

	void add(const std::string &name, Command *command) {
		if (nullptr == command) {
			_log->log(LogLevel::ERROR, "<CommandManager> Tried to register null command to %s",
				name.c_str());
			return;
		}

		_commands[name] = command;
	}

	bool execute(const std::string &commandLine);
	bool execute(const std::vector<std::string> &arguments);

	//Helper to create a new CommandAction from a command line
	CommandAction *newCommandAction(const std::string &name, const std::string &commandLine);

	void logTo(Log *log) const override;

	void writeJson(FILE *file) const {
		for (const CommandHistory &c : _commandHistory) {
			c.writeJson(file);
		}
	}

private:
	const Clock &_clock;
	Log *_log;

	std::map<std::string, Command *> _commands;
	std::vector<CommandHistory> _commandHistory;
};

}
