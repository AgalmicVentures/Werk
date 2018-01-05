
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <string>
#include <vector>

#include "Werk/Utility/Action.hpp"

namespace werk
{

/**
 * Top level command abstract class.
 */
class Command
{
public:

	Command(const std::string &help) : _help(help) { }
	virtual ~Command() { }

	const std::string &help() const { return _help; }

	virtual bool execute(const std::vector<std::string> &arguments) = 0;

private:
	std::string _help;
};

/**
 * Command that does nothing. Useful as a placeholder or for testing.
 */
class NullCommand : public Command
{
public:

	NullCommand() : Command("Null command -- does nothing.") { }

	virtual bool execute(const std::vector<std::string> &/*arguments*/) override {
		return true;
	}
};

/**
 * A `Command` that executes an `Action`.
 */
class ActionCommand : public Command
{
public:
	ActionCommand(Action *action, const std::string &help) :
		Command(help), _action(action) { }

	virtual bool execute(const std::vector<std::string> &/*arguments*/) override {
		_action->execute();
		return true;
	}

private:
	Action *_action;
};

/**
 * An `Action` that executes a `Command` with certain arguments.
 */
class CommandAction : public Action
{
public:
	CommandAction(const std::string &name, Command *command) : Action(name), _command(command) { }

	std::vector<std::string> &arguments() { return _arguments; }
	const std::vector<std::string> &arguments() const { return _arguments; }

	void execute() override { _command->execute(_arguments); }

private:
	Command *_command;
	std::vector<std::string> _arguments;
};

}
