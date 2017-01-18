
#include "HelpCommand.hpp"

#include "CommandManager.hpp"

namespace werk
{

bool HelpCommand::execute(const std::vector<std::string> &) {
	for (auto i = _commandManager->commands().begin(); i != _commandManager->commands().end(); ++i) {
		_log->log(LogLevel::INFO, "  %16s   %s", i->first.c_str(), i->second->help().c_str());
	}
	return true;
}

}
