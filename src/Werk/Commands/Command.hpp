
#pragma once

#include <string>
#include <vector>

namespace werk
{

class Command
{
public:

	Command(const std::string &help) : _help(help) { }

	const std::string &help() const { return _help; }

	virtual bool execute(const std::vector<std::string> &arguments) = 0;

private:
	std::string _help;
};

class NullCommand : public Command
{
public:

	NullCommand() : Command("Null command -- does nothing.") { }

	virtual bool execute(const std::vector<std::string> &/*arguments*/) {
		return true;
	}
};

}
