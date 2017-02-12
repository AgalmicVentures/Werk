
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
