
#pragma once

#include "Werk/Utility/Action.hpp"

#include "CommandManager.hpp"

namespace werk
{

/**
 * Writes the profiles from a given profile manager to a given path.
 */
class WriteCommandLogAction : public Action
{
public:
	WriteCommandLogAction(const std::string &name, Log *log, const CommandManager &commandManager, const std::string &path) :
		Action(name), _log(log), _commandManager(commandManager), _path(path) { }

	void execute() override {
		FILE *file = std::fopen(_path.c_str(), "w");
		if (nullptr == file) {
			_log->log(LogLevel::INFO, "Failed to write command log to %s", _path.c_str());
			return;
		}

		_commandManager.writeJson(file);
		std::fclose(file);
	}

private:
	Log *_log;
	const CommandManager &_commandManager;
	const std::string _path;
};

}
