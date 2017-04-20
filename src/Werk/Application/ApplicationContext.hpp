
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Config/Config.hpp"
#include "Werk/Config/IniConfigSource.hpp"
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Logging/LogManager.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/OS/DynamicLibraryManager.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/ActionQueue.hpp"
#include "Werk/Threading/BackgroundThread.hpp"
#include "Werk/Threading/Scheduler.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class IpcConsoleServer;

/**
 * Standard application context with all the basic components.
 *
 * NOTE: It is not necessary to use this; components may be used a-la-carte.
 */
class ApplicationContext : public Loggable
{
public:

	ApplicationContext(const std::string &logFilePath);
	~ApplicationContext();

	bool isShutdown();
	void shutdown();

	//Optional main loop
	void run(Action *mainAction=&NULL_ACTION);

	void logTo(Log *log) const override;

	//Configuration
	size_t processorCount() const { return _processorCount; }
	const std::string &instanceId() { return _instanceId; }
	bool isDebug() const { return _debug; }
	bool isRealTime() const { return _realTime; }
	bool isSimulation() const { return _simulation; }
	const std::string &temporaryPath() const { return _temporaryPath; }
	std::vector<std::string> &startupCommands() { return _startupCommands; }
	const std::vector<std::string> &startupCommands() const { return _startupCommands; }
	std::vector<std::string> &shutdownCommands() { return _shutdownCommands; }
	const std::vector<std::string> &shutdownCommands() const { return _shutdownCommands; }
	DynamicLibraryManager &dynamicLibraryManager() { return _dynamicLibraryManager; }
	const DynamicLibraryManager &dynamicLibraryManager() const { return _dynamicLibraryManager; }

	//Main thread
	const Clock &realTimeClock() const { return _realTimeClock; }
	const Clock *clock() const { return _clock; }
	uint64_t upateId() const { return _updateId; }
	ProfileManager &profileManager() { return _profileManager; }
	const ProfileManager &profileManager() const { return _profileManager; }
	ActionQueue<> &foregroundActionQueue() { return _backgroundActionQueue; }
	const ActionQueue<> &foregroundActionQueue() const { return _backgroundActionQueue; }
	std::vector<Action *> &shutdownActions() { return _shutdownActions; }
	const std::vector<Action *> &shutdownActions() const { return _shutdownActions; }

	//Background thread and tasks
	BackgroundThread &backgroundThread() { return _backgroundThread; }
	const BackgroundThread &backgroundThread() const { return _backgroundThread; }
	ActionQueue<> &backgroundActionQueue() { return _backgroundActionQueue; }
	const ActionQueue<> &backgroundActionQueue() const { return _backgroundActionQueue; }
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

private:
	//Configuration
	size_t _processorCount;
	uint64_t _physicalMemorySize;
	uint64_t _pageSize;
	std::string _instanceId;
	bool _debug;
	bool _realTime;
	bool _simulation;
	std::string _temporaryPath;
	std::vector<std::string> _startupCommands;
	std::vector<std::string> _shutdownCommands;
	DynamicLibraryManager _dynamicLibraryManager;

	//Foreground thread state
	Clock _realTimeClock;
	Clock *_clock;
	uint64_t _updateId = 0;
	ProfileManager _profileManager;
	ActionQueue<> _foregroundActionQueue { "ForegroundActionQueue" };
	std::vector<Action *> _shutdownActions;

	//Shared state
	Latch<volatile bool> _quitting;

	//Background thread state
	BackgroundThread _backgroundThread { &_profileManager };
	ActionQueue<> _backgroundActionQueue { "BackgroundActionQueue" };
	LogManager _logManager;
	AsyncLog *_stdoutLog;
	AsyncLog *_log;
	Config *_config;
	CommandManager *_commandManager;
	std::unique_ptr<IpcConsoleServer> _consoleServer;
	Scheduler *_scheduler;
};

}
