
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Config/Config.hpp"
#include "Werk/Config/IniConfigSource.hpp"
#include "Werk/Console/ConsoleCommandReceiver.hpp"
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Logging/LogManager.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/Math/Random.hpp"
#include "Werk/OS/DynamicLibraryManager.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/ActionQueue.hpp"
#include "Werk/Threading/BackgroundThread.hpp"
#include "Werk/Threading/Scheduler.hpp"
#include "Werk/Utility/Attributes.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class IpcConsoleServer;

/**
 * Standard application context with all the basic components.
 *
 * NOTE: It is not necessary to use this; components may be used a-la-carte.
 */
class Context : public Configurable, public Loggable
{
public:

	//The first config path is mandatory; the secondary is optional
	Context(const std::string &configPath, const char *secondaryConfigPath=nullptr, bool dryRun=false);
	virtual ~Context();

	virtual bool reloadConfig(const Config &config) override;

	CHECKED int exitCode() { return _exitCode.load(); }
	void setExitCode(int code) { _exitCode.store(code); }
	CHECKED bool isQuitting() { return _quitting.value(); }
	void quit() { _quitting.set(); }
	CHECKED bool isShutdown();
	void shutdown();

	//Optional main loop with exit code
	CHECKED int run(Action *mainAction=&NULL_ACTION);

	void logTo(Log *log) const override;

	//Configuration
	CHECKED size_t processorCount() const { return _processorCount; }
	CHECKED const std::string &instanceId() { return _instanceId; }
	CHECKED bool isDebug() const { return _debug; }
	CHECKED bool isDryRun() const { return _dryRun; }
	CHECKED bool isRealTime() const { return _realTime; }
	CHECKED bool isSimulation() const { return _simulation; }
	CHECKED const std::string &temporaryPath() const { return _temporaryPath; }
	CHECKED const char *profilesPath() const { return _profilesPath; }
	CHECKED std::vector<std::string> &startupCommands() { return _startupCommands; }
	CHECKED const std::vector<std::string> &startupCommands() const { return _startupCommands; }
	CHECKED std::vector<std::string> &shutdownCommands() { return _shutdownCommands; }
	CHECKED const std::vector<std::string> &shutdownCommands() const { return _shutdownCommands; }
	CHECKED DynamicLibraryManager &dynamicLibraryManager() { return _dynamicLibraryManager; }
	CHECKED const DynamicLibraryManager &dynamicLibraryManager() const { return _dynamicLibraryManager; }

	//Main thread
	CHECKED const Clock &realTimeClock() const { return _realTimeClock; }
	CHECKED const Clock *clock() const { return _clock; }
	CHECKED uint64_t upateId() const { return _updateId; }
	CHECKED Random &random() { return _random; }
	CHECKED const Random &random() const { return _random; }
	CHECKED ProfileManager &profileManager() { return _profileManager; }
	CHECKED const ProfileManager &profileManager() const { return _profileManager; }
	CHECKED ActionQueue<> &foregroundActionQueue() { return _backgroundActionQueue; }
	CHECKED const ActionQueue<> &foregroundActionQueue() const { return _backgroundActionQueue; }
	CHECKED std::vector<Action *> &shutdownActions() { return _shutdownActions; }
	CHECKED const std::vector<Action *> &shutdownActions() const { return _shutdownActions; }
	CHECKED Profile &interUpdateProfile() { return _interUpdateProfile; }
	CHECKED const Profile &interUpdateProfile() const { return _interUpdateProfile; }
	CHECKED Profile &updateProfile() { return _updateProfile; }
	CHECKED const Profile &updateProfile() const { return _updateProfile; }

	//Background thread and tasks
	CHECKED BackgroundThread &backgroundThread() { return _backgroundThread; }
	CHECKED const BackgroundThread &backgroundThread() const { return _backgroundThread; }
	CHECKED ActionQueue<> &backgroundActionQueue() { return _backgroundActionQueue; }
	CHECKED const ActionQueue<> &backgroundActionQueue() const { return _backgroundActionQueue; }
	CHECKED LogManager &logManager() { return _logManager; }
	CHECKED const LogManager &logManager() const { return _logManager; }
	CHECKED AsyncLog *stdoutLog() { return _stdoutLog; }
	CHECKED const AsyncLog *stdoutLog() const { return _stdoutLog; }
	CHECKED AsyncLog *realTimeLog() { return _realTimeLog; }
	CHECKED const AsyncLog *realTimeLog() const { return _realTimeLog; }
	CHECKED AsyncLog *log() { return _log; }
	CHECKED const AsyncLog *log() const { return _log; }
	CHECKED Config *config() { return _config; }
	CHECKED const Config *config() const { return _config; }
	CHECKED CommandManager *commandManager() { return _commandManager; }
	CHECKED const CommandManager *commandManager() const { return _commandManager; }

private:
	//Configuration
	size_t _processorCount;
	uint64_t _physicalMemorySize;
	uint64_t _pageSize;
	std::string _instanceId;
	bool _debug;
	bool _dryRun;
	bool _realTime;
	bool _simulation;
	std::string _temporaryPath;
	std::vector<std::string> _startupCommands;
	std::vector<std::string> _shutdownCommands;
	DynamicLibraryManager _dynamicLibraryManager;
	//Dyanimc configuration
	uint64_t _longUpdateAlertThreshold = 500'000;
	uint64_t _longUpdateWarningThreshold = 1'000'000;
	uint64_t _longUpdateErrorThreshold = 2'000'000;
	uint64_t _longUpdateCriticalThreshold = 5'000'000;

	//Foreground thread state
	Clock _realTimeClock;
	Clock *_clock;
	uint64_t _startupTime = 0;
	uint64_t _updateId = 0;
	Random _random;
	ProfileManager _profileManager;
	const char *_profilesPath = nullptr;
	ActionQueue<> _foregroundActionQueue { "ForegroundActionQueue" };
	std::vector<Action *> _shutdownActions;
	Profile _interUpdateProfile;
	Profile _updateProfile;

	//Shared state
	std::atomic<int> _exitCode;
	Latch<std::atomic<bool> > _quitting;

	//Background thread state
	BackgroundThread _backgroundThread { &_profileManager };
	ActionQueue<> _backgroundActionQueue { "BackgroundActionQueue" };
	LogManager _logManager;
	AsyncLog *_stdoutLog;
	AsyncLog *_realTimeLog;
	AsyncLog *_log;
	Config *_config;
	CommandManager *_commandManager;
	std::unique_ptr<IpcConsoleServer> _consoleServer;
	std::unique_ptr<ConsoleCommandReceiver> _consoleCommandReceiver;
	Scheduler *_scheduler;
};

}
