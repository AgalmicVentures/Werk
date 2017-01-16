
#pragma once

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Config/Config.hpp"
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

namespace werk
{

class ApplicationContext
{
public:

	ApplicationContext(const std::string &logFilePath);

	//Background thread and tasks
	BackgroundThread &backgroundThread() { return _backgroundThread; }
	const BackgroundThread &backgroundThread() const { return _backgroundThread; }
	AsyncLog *log() { return _log; }
	const AsyncLog *log() const { return _log; }
	AsyncLog *stdoutLog() { return _stdoutLog; }
	const AsyncLog *stdoutLog() const { return _stdoutLog; }
	Config *config() { return _config; }
	const Config *config() const { return _config; }

	const Clock &realTimeClock() const { return _realTimeClock; }
	ProfileManager &profileManager() { return _profileManager; }
	const ProfileManager &profileManager() const { return _profileManager; }
	CommandManager *commandManager() { return _commandManager; }
	const CommandManager *commandManager() const { return _commandManager; }

private:
	BackgroundThread _backgroundThread;
	AsyncLog *_log;
	AsyncLog *_stdoutLog;
	Config *_config;
	CommandManager *_commandManager;

	Clock _realTimeClock;
	ProfileManager _profileManager;
};

}
