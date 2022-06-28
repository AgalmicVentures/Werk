

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

#include "Context.hpp"

#include <boost/algorithm/string.hpp>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <unistd.h>

#include "Werk/Commands/CommandAlias.hpp"
#include "Werk/Commands/ConsoleClientsCommand.hpp"
#include "Werk/Commands/SegfaultCommand.hpp"
#include "Werk/Commands/WriteCommandLogAction.hpp"
#include "Werk/Console/ConsoleCommandReceiver.hpp"
#include "Werk/Console/IpcConsoleServer.hpp"
#include "Werk/Data/Csv/CsvTimeSeries.hpp"
#include "Werk/Data/JitterTimeSeries.hpp"
#include "Werk/Data/Pcap/PcapParser.hpp"
#include "Werk/Data/TimeSeriesReplayer.hpp"
#include "Werk/OS/CpuMask.hpp"
#include "Werk/OS/Hardware.hpp"
#include "Werk/OS/OS.hpp"
#include "Werk/OS/Signals.hpp"
#include "Werk/Profiling/WriteProfilesAction.hpp"
#include "Werk/Threading/Watchdog.hpp"

namespace werk
{

Context::Context(const std::string &configPath, const char *secondaryConfigPath, bool dryRun) :
	_dryRun(dryRun),
	_interUpdateProfile("InterUpdate", 1000, 1000000),
	_updateProfile("Update", 1000, 1000000)
{
	//And let's pull ourselves up by our bootstraps...

	/********** Pre-Initialization **********/

	//Setup handlers for certain signals
	if (!setupSegfaultHandler()) {
		std::fprintf(stderr, "Failed to setup segfault handler -- may exit uncleanly\n");
	}

	_realTimeClock.setEpochTime();
	_startupTime = _realTimeClock.time();
	_backgroundThread.addTask(&_backgroundActionQueue);
	_profileManager.add(&_interUpdateProfile);
	_profileManager.add(&_updateProfile);

	/********** Stdout Log **********/

	_stdoutLog = new AsyncLog("StdoutLog", &_backgroundThread.backgroundClock(), stdout, &_realTimeClock);
	assert(_logManager.add(_stdoutLog));
	_backgroundThread.addTask(_stdoutLog);

	/********** Config **********/

	_config = new Config("Config", _stdoutLog);
	_config->addConfigurable(this);

	//Load the primary config
	_stdoutLog->log(LogLevel::INFO, "<Config> Loading %s...", configPath.c_str());
	IniConfigSource *primaryConfigSource = new IniConfigSource(configPath, _stdoutLog);
	_config->addConfigSource(primaryConfigSource);

	IniConfigSource *secondaryConfigSource = nullptr;
	if (nullptr != secondaryConfigPath) {
		_stdoutLog->log(LogLevel::INFO, "<Config> Loading %s...", secondaryConfigPath);
		secondaryConfigSource = new IniConfigSource(secondaryConfigPath, _stdoutLog);
		_config->addConfigSource(secondaryConfigSource);
	}

	//Synchronously execute the reload since the values are needed for the next step
	_config->reloadConfig();
	_config->execute();

	//Register other config files
	std::vector<IniConfigSource *> iniConfigSources;
	std::vector<std::string> configPaths;
	_config->getStrings("Application.ConfigPaths", configPaths, nullptr,
		"Paths to config files, with the final one having highest priority (primary config still overrides all)");
	for (const auto &path : configPaths) {
		iniConfigSources.push_back(new IniConfigSource(path, _stdoutLog));
		_config->addConfigSource(iniConfigSources[iniConfigSources.size() - 1]);
	}

	//Put the primary config as the highest priority
	iniConfigSources.push_back(primaryConfigSource);
	if (nullptr != secondaryConfigSource) {
		iniConfigSources.push_back(secondaryConfigSource);
	}

	//Synchronously reload with everything
	_config->clearConfigSources();
	for (IniConfigSource *iniConfigSource : iniConfigSources) {
		_config->addConfigSource(iniConfigSource);
	}
	_config->reloadConfig();
	_config->execute();

	//Finally, with all files loaded, start reloading in the background
	_backgroundThread.addTask(_config);
	_stdoutLog->logRaw(LogLevel::SUCCESS, "<Config> Initialized.");

	//And begin updating the background thread at the configured interval
	const uint64_t backgroundThreadIntervalNs = _config->getTimeAmount(
		"Application.BackgroundThreadInterval", _backgroundThread.intervalNs(),
		"Interval between background updates");
	_backgroundThread.setIntervalNs(backgroundThreadIntervalNs);

	/********** Real Time Log **********/

	const char *logPath = _config->getString("Application.LogPath", nullptr,
		"Path to write the main log with wall clock times (or none to use stdout)");
	FILE *file = nullptr == logPath ? stdout : std::fopen(logPath, "a");
	if (nullptr == file) {
		_stdoutLog->logRaw(LogLevel::ERROR, "Could not open log file, redirecting to stderr.");
		file = stderr;
	}

	const bool updateClockOnLog = _config->getBool("Application.UpdateClockOnLog", true,
		"Updates the clock one each log line");
	_realTimeLog = new AsyncLog("RealTimeLog", &_realTimeClock, file, updateClockOnLog ? &_realTimeClock : nullptr);
	assert(_logManager.add(_realTimeLog));
	_backgroundThread.addTask(_realTimeLog);
	_config->setLog(_realTimeLog);
	_realTimeLog->logRaw(LogLevel::SUCCESS, "<RealTimeLog> Initialized.");

	/********** Determine If Real Time **********/

	//Real time mode or not? In non-real time mode, a secondary clock is instantiated
	_realTime = _config->getBool("Application.RealTime", true,
		"Indicates whether the application is running in real time or from a different clock");
	if (_realTime) {
		_clock = &_realTimeClock;
		_log = _realTimeLog;
	} else {
		_clock = new Clock();

		/********** Main Log (Historical Time) **********/

		const char *historicalLogPath = _config->getString("Application.HistoricalLogPath", nullptr,
			"Path to write historical logs (that is, logs timed against the simulated clock)");
		FILE *historicalFile = nullptr == historicalLogPath ? stdout : std::fopen(historicalLogPath, "a");
		if (nullptr == historicalFile) {
			_stdoutLog->logRaw(LogLevel::ERROR, "Could not open historical log file, redirecting to stderr.");
			historicalFile = stderr;
		}

		_log = new AsyncLog("HistoricalLog", _clock, historicalFile);
		assert(_logManager.add(_log));
		_backgroundThread.addTask(_log);
		_config->setLog(_log);
		_log->logRaw(LogLevel::SUCCESS, "<HistoricalLog> Initialized.");
	}

	for (IniConfigSource *iniConfigSource : iniConfigSources) {
		iniConfigSource->setLog(_log);
	}

	/********** Detect System State Now That Log Is Setup **********/

	/***** Software State *****/

	//PID's
	const pid_t pid = getpid();
	const pid_t ppid = getppid();

	//Current working directory
	char currentWorkingDirectory[1024];
	if (nullptr == getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory))) {
		std::strcpy(currentWorkingDirectory, "Error getting current working directory.");
	}

	//Hostname
	char hostname[1024];
	if (0 != gethostname(hostname, sizeof(hostname))) {
		std::strcpy(hostname, "Error getting hostname.");
	}

	_log->log(LogLevel::JSON, "{\"type\":\"startup.software\",\"os\":\"%s\",\"cwd\":\"%s\",\"hostname\":\"%s\"}",
		getOperatingSystemName(), currentWorkingDirectory, hostname);
	_realTimeLog->log(LogLevel::JSON, "{\"type\":\"startup.pid\",\"pid\":%ld,\"ppid\":%ld}", pid, ppid);

	/***** Hardware State *****/

	//Processor
	_processorCount = getProcessorCount();

	//RAM
	_physicalMemorySize = getPhysicalMemorySize();
	_pageSize = getPageSize();

	_log->log(LogLevel::JSON, "{\"type\":\"startup.hardware\",\"processorCount\":%zu,\"physicalMemory\":%" PRIu64 ",\"pageSize\":%" PRIu64 "}",
		_processorCount, _physicalMemorySize, _pageSize);

	/********** Configure Existing Components Now That Log Is Setup **********/

	//Set the instance ID
	_instanceId = _config->getString("Application.InstanceID", "", "ID of this instance of the application");

	_debug = _config->getBool("Application.Debug", false,
		"Indicates whether the application should output additional debug information (even in release builds)");

	const bool configDryRun = _config->getBool("Application.DryRun", false,
		"Indicates whether the application should do a dry run (no update cycles; immediate shutdown)");
	_dryRun = _dryRun || configDryRun;
	if (_dryRun) {
		_log->log(LogLevel::INFO, "<Context> Executing dry run (no update cycles)");
		quit(); //Deferred -- sets the quit flag so the update loop is not run -- everything else runs as normal
	}

	_simulation = _config->getBool("Application.Simulation", false,
		"Indicates whether the application is running a simulation or not");

	//Set the temporary path (but do not allow an empty string, or logging to / )
	_temporaryPath = _config->getString("Application.TemporaryPath", "/tmp", "Path to put temporary files in");
	if (_temporaryPath.length() < 2) {
		_stdoutLog->logRaw(LogLevel::ERROR, "Invalid temporary path; defaulting to /tmp.");
		_temporaryPath = "/tmp";
	}

	//Setup profiling output
	_profilesPath = _config->getString("Application.ProfilesPath", nullptr, "Path to write profiling data on exit");
	//Rest moved a few lines below to allow adding a command

	/********** Commands **********/

	//Initialize command manager
	_commandManager = new CommandManager(_backgroundThread.backgroundClock(), _log);

	if (nullptr != _profilesPath) {
		WriteProfilesAction *writeProfilesAction = new WriteProfilesAction("WriteProfiles", _log, _profileManager, _profilesPath);
		_shutdownActions.push_back(writeProfilesAction);

		//TODO: it would be nice if a path could be passed to this -- write a custom command?
		_commandManager->add("writeProfiles", new ActionCommand(
			writeProfilesAction,
			"Writes the profiles immediately."));
	}

	_commandManager->add("app", new ActionCommand(
		new LogAction("LogContext", this, _log),
		"Logs information about the application."));
	_commandManager->add("bg", new ActionCommand(
		new LogAction("LogBackgroundTasks", &_backgroundThread, _log),
		"Logs information about background tasks."));
	_commandManager->add("logs", new ActionCommand(
		new LogAction("LogLogs", &_logManager, _log),
		"Logs information about existing log files."));
	_commandManager->add("reload", new ActionCommand(
		_config->getReloadConfigAction(),
		"Reloads the configuration."));

	Action *quitAction = new SetLatchAction<std::atomic<bool> >("Quit", _quitting);
	_commandManager->add("quit", new ActionCommand(
		quitAction,
		"Quits the application cleanly."));

	//Some commands are only available in simulation
	if (_simulation) {
		_commandManager->add("segfault", new SegfaultCommand(_log));
	}

	_log->logRaw(LogLevel::SUCCESS, "<CommandManager> Initialized.");

	//Setup command aliases configured like so:
	//Application.CommandAliases=abc:echo Abc!;123:echo 123!
	std::vector<std::string> commandAliases;
	_config->getStrings("Application.CommandAliases", commandAliases,
		nullptr, nullptr, ";");
	for (const auto &commandAlias : commandAliases) {
		//Split on :
		const std::size_t colonPos = commandAlias.find(':');
		if (std::string::npos == colonPos) {
			//No value found, error
			_log->log(LogLevel::ERROR, "Could not parse command alias: %s",
				commandAlias.c_str());
			continue;
		}

		//Copy the key and value into the map
		_commandManager->add(commandAlias.substr(0, colonPos),
			new CommandAlias(*_commandManager, commandAlias.substr(colonPos + 1)));
	}

	//Setup command logging
	const char *commandLogPath = _config->getString("Application.CommandLogPath", nullptr,
		"Path to log commands on exit");
	if (nullptr != commandLogPath) {
		_shutdownActions.push_back(new WriteCommandLogAction("WriteCommandLog", _log, *_commandManager, commandLogPath));
	}

	/********** Console **********/

	//Consoles are only available for real time runs
	if (_realTime) {
		const char *ipcConsoleName = _config->getString("Application.IpcConsoleName", nullptr,
			"Name of the shared memory queue for the console (often in /dev/shm)");
		if (nullptr != ipcConsoleName) {
			_consoleServer.reset(new IpcConsoleServer(ipcConsoleName, _log, &_realTimeClock));
			_commandManager->add("consoles", new ConsoleClientsCommand(*_consoleServer, _log, &_realTimeClock));

			//Create the receiver
			//TODO: move to the background thread
			_consoleCommandReceiver.reset(new ConsoleCommandReceiver("IpcConsoleServer", *_consoleServer, *_commandManager));
			//_backgroundThread.addTask(consoleCommandReceiver);
		}
	}

	/********** Scheduler **********/

	//TODO: set this up with the simulated clock so it can be used in simulations
	_scheduler = new Scheduler("Scheduler", &_backgroundThread.mainClock());
	_backgroundThread.addTask(_scheduler);

	//Configure scheduled commands
	//TODO: consider defering this until later, once the user has setup everything they need?
	/*
	const char *scheduledCommandsStr = _config->getString("Application.ScheduledCommands");
	if (nullptr != scheduledCommandsStr) {
		//Split and add each command
		std::vector<std::string> scheduledCommands;
		boost::split(scheduledCommands, scheduledCommandsStr, boost::is_any_of(";"));
		for (const auto &command : scheduledCommands) {
			(void) command;
			//TODO: parse the time
			//TODO: get the command action
			//TODO: schedule it
		}
	}
	*/

	/********** Finish Initialization **********/

	//Setup remaining signals
	//SIGHUP -> reload config
	if (!setupSignalHandler(SIGHUP, _config->getReloadConfigAction())) {
		_log->log(LogLevel::ERROR, "Failed to setup SIGHUP handler.");
	}
	if (!setupSignalHandler(SIGINT, quitAction)) {
		_log->log(LogLevel::ERROR, "Failed to setup SIGINT handler.");
	}

	//Load and run startup commands
	//TODO: consider defering this until later, once the user has setup everything they need?
	const char *startupCommandsStr = _config->getString("Application.StartupCommands", nullptr,
		"Commands to run on startup");
	if (nullptr != startupCommandsStr) {
		//Split and run each command
		boost::split(_startupCommands, startupCommandsStr, boost::is_any_of(";"));
		for (const auto &command : _startupCommands) {
			_commandManager->execute(command);
		}
	}

	_log->logRaw(LogLevel::JSON, "{\"type\":\"startup.initialized\"}");
}

Context::~Context()
{
	//Shut down, if not already done
	if (!isShutdown()) {
		shutdown();
	}
}

bool Context::reloadConfig(const Config &config)
{
	_longUpdateAlertThreshold = config.getTimeAmount("Application.LongUpdateAlertThreshold", 250'000,
		"Threshold to log an alert about the duration of an update cycle");
	_longUpdateWarningThreshold = config.getTimeAmount("Application.LongUpdateWarningThreshold", 1'000'000,
		"Threshold to log a warning about the duration of an update cycle");
	_longUpdateErrorThreshold = config.getTimeAmount("Application.LongUpdateErrorThreshold", 2'000'000,
		"Threshold to log an error about the duration of an update cycle");
	_longUpdateCriticalThreshold = config.getTimeAmount("Application.LongUpdateCriticalThreshold", 5'000'000,
		"Threshold to log a critical about the duration of an update cycle");

	return true;
}

void Context::logTo(Log *log) const
{
	log->log(LogLevel::INFO, "<Context>      Debug: %s", _debug ? "Yes" : "No");
	log->log(LogLevel::INFO, "<Context>  Real Time: %s", _realTime ? "Yes" : "No");
	log->log(LogLevel::INFO, "<Context> Simulation: %s", _simulation ? "Yes" : "No");
}

bool Context::isShutdown()
{
	return _backgroundThread.stopped();
}

void Context::shutdown()
{
	//Don't shut down twice
	if (isShutdown()) {
		std::fprintf(stderr, "Context::shutdown - Already shut down.\n");
		return;
	}

	//Load and run shutdown commands
	const char *shutdownCommandsStr = _config->getString("Application.ShutdownCommands");
	if (nullptr != shutdownCommandsStr) {
		//Split on ; and run each command
		boost::split(_shutdownCommands, shutdownCommandsStr, boost::is_any_of(";"));
		for (const auto &command : _shutdownCommands) {
			_commandManager->execute(command);
		}
	}

	//Run shutdown actions
	_log->logRaw(LogLevel::JSON, "{\"type\":\"shutdown.shutdownActions\"}");
	for (Action *action : _shutdownActions) {
		action->execute();
	}
	_log->logRaw(LogLevel::JSON, "{\"type\":\"shutdown.shutdownActionsComplete\"}");

	_log->logRaw(LogLevel::JSON, "{\"type\":\"shutdown.shuttingDown\"}");
	_backgroundThread.stop();

	//Cleanup the console
	_consoleServer.reset();
}

int Context::run(Action *mainAction)
{
	//Reload the configuration once before starting
	if (!_config->reloadConfigurables()) {
		_log->log(LogLevel::CRITICAL, "<Context> Failed initial config reload. Quitting...");
		return 1;
	}

	//Setup the background watchdog timer
	const uint64_t watchdogInterval = _config->getTimeAmount("Application.WatchdogInterval", 0,
		"Interval of the main thread watchdog (ns)");
	const uint64_t watchdogAllowedMisses = _config->getUint64("Application.WatchdogAllowedMisses", 1,
		"Number of times the main thread can miss the watchdog");
	//TODO: configurable watchdog action
	Watchdog *watchdog = new Watchdog("Watchdog", &_backgroundThread.backgroundClock(),
		new LogAction("WatchdogWarning", new StringLoggable("Main thread missed watchdog timer! It may be hung or dead.", LogLevel::CRITICAL), _log),
		watchdogInterval, watchdogAllowedMisses);

	if (0 != watchdogInterval) {
		if (watchdogInterval <= 2 * _backgroundThread.intervalNs()) {
			_log->logRaw(LogLevel::ERROR, "Watchdog interval is less than double background thread interval.");
		}
		_backgroundThread.addTask(watchdog);
	}

	//Setup time series replayer for historical data if not real time
	Action *actionToRun = mainAction;
	TimeSeriesReplayer *timeSeriesReplayer = nullptr;
	if (!_realTime) {
		_log->logRaw(LogLevel::JSON, "{\"type\":\"timeSeriesReplayer.initializing\"}");
		uint64_t timeout = _config->getTimeAmount("Application.HistoricalDataTimeout", 0,
			"Maximum time between events (allows updating at a higher frequency than the data)");
		timeSeriesReplayer = new TimeSeriesReplayer("TimeSeriesReplayer", _clock, _quitting, mainAction, _log, timeout);
		actionToRun = timeSeriesReplayer;

		//TODO: consider a per source value for this
		uint64_t jitterMin = _config->getTimeAmount("Application.HistoricalDataJitterMin", 0,
			"Minimum jitter to add to event times");
		uint64_t jitterMax = _config->getTimeAmount("Application.HistoricalDataJitterMax", 0,
			"Maximum jitter to add to event times");
		if (jitterMin > jitterMax) {
			_log->log(LogLevel::WARNING, "Jitter min is more than max, swapping");
			std::swap(jitterMin, jitterMax);
		}
		bool useJitter = jitterMax > 0;

		//Setup data sources
		std::vector<std::string> dataSourcePaths;
		_config->getStrings("Application.HistoricalDataSources", dataSourcePaths,
			nullptr, "Paths to historical data sources");
		for (const std::string &dataSourcePath : dataSourcePaths) {
			TimeSeries *timeSeries = nullptr;
			if (boost::algorithm::ends_with(dataSourcePath, ".csv")) {
				//TODO: this demands more configurability
				CsvParser *csvParser = new CsvParser();
				if (csvParser->open(dataSourcePath)) {
					CsvTimeSeries *csvTimeSeries = new CsvTimeSeries(dataSourcePath, *csvParser); //Use path as name
					timeSeries = csvTimeSeries;
				} else {
					_log->log(LogLevel::ERROR, "Failed to open historical data source: %s.", dataSourcePath.c_str());
					delete csvParser;
				}
			} else if (boost::algorithm::ends_with(dataSourcePath, ".pcap")) {
				PcapParser *pcapParser = new PcapParser(dataSourcePath); //Use path as name
				if (pcapParser->open(dataSourcePath)) {
					timeSeries = pcapParser;
				} else {
					_log->log(LogLevel::ERROR, "Failed to open historical data source: %s.", dataSourcePath.c_str());
					delete pcapParser;
				}
			} else {
				_log->log(LogLevel::ERROR, "Unknown historical data source file type: %s.", dataSourcePath.c_str());
			}

			//Skip things that fail to load
			if (nullptr == timeSeries) {
				continue;
			}

			//Optionally add jitter
			if (useJitter) {
				JitterTimeSeries *jitterTimeSeries = new JitterTimeSeries(_random, timeSeries, jitterMin, jitterMax);
				timeSeries = jitterTimeSeries;
			}

			//Add it to the replayer
			timeSeriesReplayer->addDataSource(timeSeries);
			_log->log(LogLevel::SUCCESS, "Opened historical data source: %s.", dataSourcePath.c_str());
		}

		_log->logRaw(LogLevel::JSON, "{\"type\":\"timeSeriesReplayer.initialized\"}");
	}

	//Run the main loop
	_log->logRaw(LogLevel::JSON, "{\"type\":\"mainLoop.enter\"}");
	while (!_quitting.value()) {
		_realTimeClock.setEpochTime();
		_updateId += 1;
		actionToRun->execute();

		//Run other queued actions after the main action
		_foregroundActionQueue.execute();

		//Update stats
		_interUpdateProfile.restart(_clock->time());

		PROFILE_START(_updateProfile);

		//Receive commands and update
		//TODO: Move to the background thread
		if (nullptr != _consoleServer) {
			_consoleCommandReceiver->execute();
			_consoleServer->update();
		}

		//Made it through another loop, update background objects -- update clock, reset the watchdog
		_backgroundThread.setMainClockTime(_realTimeClock.time());
		watchdog->reset();

		//Check the update time
		const int64_t updateTime = PROFILE_STOP(_updateProfile);
		if (_longUpdateAlertThreshold <= updateTime) {
			const LogLevel logLevel =
				_longUpdateCriticalThreshold <= updateTime ? LogLevel::CRITICAL :
				_longUpdateErrorThreshold <= updateTime ? LogLevel::ERROR :
				_longUpdateWarningThreshold <= updateTime ? LogLevel::WARNING :
				LogLevel::ALERT;
			_log->log(logLevel, "<Context> Update cycle took a long time: %" PRIi64 "ns",
				updateTime);
		}
	}
	_log->log(LogLevel::JSON, "{\"type\":\"mainLoop.exit\",\"updates\":%" PRIu64 "}", _updateId);

	//Shut down and return the exit code
	shutdown();
	return exitCode();
}

}
