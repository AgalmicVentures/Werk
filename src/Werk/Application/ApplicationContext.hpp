
#pragma once

#include <string>
#include <vector>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Config/Config.hpp"
#include "Werk/Config/IniConfigSource.hpp"
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Logging/LogManager.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

namespace werk
{

/**
 * Standard application context with all the basic components.
 */
class ApplicationContext
{
public:

	ApplicationContext(const std::string &logFilePath);
	~ApplicationContext();

	void shutdown();

	//Background thread and tasks
	BackgroundThread &backgroundThread() { return _backgroundThread; }
	const BackgroundThread &backgroundThread() const { return _backgroundThread; }
	LogManager &logManager() { return _logManager; }
	const LogManager &logManager() const { return _logManager; }
	AsyncLog *stdoutLog() { return _stdoutLog; }
	const AsyncLog *stdoutLog() const { return _stdoutLog; }
	AsyncLog *log() { return _log; }
	const AsyncLog *log() const { return _log; }
	Config *config() { return _config; }
	const Config *config() const { return _config; }
	CommandManager *commandManager() { return _commandManager; }
	const CommandManager *commandManager() const { return _commandManager; }

	//Main thread
	const Clock &realTimeClock() const { return _realTimeClock; }
	ProfileManager &profileManager() { return _profileManager; }
	const ProfileManager &profileManager() const { return _profileManager; }
	std::vector<Action *> &shutdownActions() { return _shutdownActions; }
	const std::vector<Action *> &shutdownActions() const { return _shutdownActions; }

private:
	BackgroundThread _backgroundThread;
	LogManager _logManager;
	AsyncLog *_stdoutLog;
	AsyncLog *_log;
	Config *_config;
	CommandManager *_commandManager;

	Clock _realTimeClock;
	ProfileManager _profileManager;
	std::vector<Action *> _shutdownActions;
};

}
