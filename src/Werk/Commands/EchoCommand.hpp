
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
		//Join all but the first argument since the command name needn't be echoed
		_log->logRaw(_level, boost::algorithm::join(boost::make_iterator_range(arguments.begin() + 1, arguments.end()), " ").c_str());
		return true;
	}

private:
	Log *_log;
	LogLevel _level;
};

}
