
#include "ApplicationContext.hpp"

#include <cstdio>

namespace werk
{

ApplicationContext::ApplicationContext(const std::string &logFilePath)
{
	_stdoutLog = new AsyncLog("StdoutLog", &_backgroundThread.backgroundClock());
	_backgroundThread.addTask(_stdoutLog);

	FILE *file = logFilePath.length() == 0 ? stdout : std::fopen(logFilePath.c_str(), "r");
	_log = new AsyncLog("Log", &_backgroundThread.backgroundClock(), file),
	_backgroundThread.addTask(_log);

	_config = new Config("Config", _log);
	_backgroundThread.addTask(_config);

	_commandManager = new CommandManager(_log);
}

}
