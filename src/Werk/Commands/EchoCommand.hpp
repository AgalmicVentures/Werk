
#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Logging/Logger.hpp"

#include "Command.hpp"

namespace werk
{

class EchoCommand : public Command
{
public:

	EchoCommand(Logger *log, LogLevel level=LogLevel::INFO) :
		Command("Echoes the arguments to the log."),
		_log(log), _level(level) { }

	virtual bool execute(const std::vector<std::string> &arguments) {
		_log->logRaw(_level, boost::algorithm::join(arguments, " ").c_str());
		return true;
	}

private:
	Logger *_log;
	LogLevel _level;
};

}
