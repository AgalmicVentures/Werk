
#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Logging/Log.hpp"

#include "Command.hpp"

namespace werk
{

/**
 * Command that echoes its arguments.
 */
class EchoCommand : public Command
{
public:

	EchoCommand(Log *log, LogLevel level=LogLevel::INFO) :
		Command("Echoes the arguments to the log."),
		_log(log), _level(level) { }

	bool execute(const std::vector<std::string> &arguments) override {
		_log->logRaw(_level, boost::algorithm::join(arguments, " ").c_str());
		return true;
	}

private:
	Log *_log;
	LogLevel _level;
};

}
