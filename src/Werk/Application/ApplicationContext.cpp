
#include "ApplicationContext.hpp"

#include <boost/algorithm/string.hpp>
#include <cstdio>

#include "Werk/Config/ReloadConfigCommand.hpp"
#include "Werk/OS/Signals.hpp"
#include "Werk/Profiling/WriteProfilesAction.hpp"

#include "QuitCommand.hpp"

namespace werk
{

ApplicationContext::ApplicationContext(const std::string &configPath)
{
	//And let's pull ourselves up by our bootstraps...

	/********** Pre-Initialization **********/

	//Setup handlers for certain signals
	setupSegfaultHandler();

	/********** Stdout Log **********/

	_stdoutLog = new AsyncLog("StdoutLog", &_backgroundThread.backgroundClock());
	_logManager.add(_stdoutLog);
	_backgroundThread.addTask(_stdoutLog);

	/********** Config **********/

	_config = new Config("Config", _stdoutLog);

	//Synchronously execute the reload since the values are needed for the next step
	_config->addConfigSource(new IniConfigSource(configPath));
	_config->reloadConfig();
	_config->execute();

	//Register other config files
	const char *configPathsStr = _config->getString("Application.ConfigPaths");
	if (nullptr != configPathsStr) {
		//Add all the new configs
		std::vector<std::string> configPaths;
		boost::split(configPaths, configPathsStr, boost::is_any_of(","));
		for (auto &path : configPaths) {
			_config->addConfigSource(new IniConfigSource(path));
		}
	}

	//Synchronously reload with everything
	_config->addConfigSource(new IniConfigSource(configPath));
	_config->reloadConfig();
	_config->execute();

	//Finally, with all files loaded, start reloading in the background
	_backgroundThread.addTask(_config);
	_stdoutLog->logRaw(LogLevel::SUCCESS, "<Config> Initialized.");

	/********** Main Log **********/

	const char *logPath = _config->getString("Application.LogPath");
	FILE *file = nullptr == logPath ? stdout : std::fopen(logPath, "a");
	if (nullptr == file) {
		_stdoutLog->logRaw(LogLevel::ERROR, "Could not open log file, redirecting to stderr.\n");
		file = stderr;
	}

	_log = new AsyncLog("Log", &_backgroundThread.backgroundClock(), file);
	_logManager.add(_log);
	_backgroundThread.addTask(_log);
	_config->setLog(_log);
	_log->logRaw(LogLevel::SUCCESS, "<Log> Initialized.");

	/********** Command Manager **********/

	_commandManager = new CommandManager(_log);
	_commandManager->add("reload", new ReloadConfigCommand(*_config));
	_commandManager->add("quit", new QuitCommand(this));
	_log->logRaw(LogLevel::SUCCESS, "<CommandManager> Initialized.");

	const char *profilesPath = _config->getString("Application.ProfilesPath");
	if (nullptr != profilesPath) {
		_log->log(LogLevel::INFO, "Writing profiles to %s on shutdown.", profilesPath);
		_shutdownActions.push_back(new WriteProfilesAction("WriteProfiles", _log, _profileManager, profilesPath));
	}

	/********** Finish Initialization **********/

	//Load and run startup commands
	//TODO: consider defering this until later, once the user has setup everything they need?
	const char *startupCommandsStr = _config->getString("Application.StartupCommands");
	if (nullptr != startupCommandsStr) {
		//Split and run each command
		boost::split(_startupCommands, startupCommandsStr, boost::is_any_of(";"));
		for (auto &command : _startupCommands) {
			_commandManager->execute(command);
		}
	}

	_log->logRaw(LogLevel::SUCCESS, "<ApplicationContext> Initialized.");
}

ApplicationContext::~ApplicationContext()
{
	//Shut down, if not already done
	if (!isShutdown()) {
		shutdown();
	}
}

bool ApplicationContext::isShutdown()
{
	return _backgroundThread.stopped();
}

void ApplicationContext::shutdown()
{
	//Don't shut down twice
	if (isShutdown()) {
		fprintf(stderr, "ApplicationContext::shutdown - Already shut down.\n");
		return;
	}

	//Load and run shutdown commands
	const char *shutdownCommandsStr = _config->getString("Application.ShutdownCommands");
	if (nullptr != shutdownCommandsStr) {
		//Split on ; and run each command
		boost::split(_shutdownCommands, shutdownCommandsStr, boost::is_any_of(";"));
		for (auto &command : _startupCommands) {
			_commandManager->execute(command);
		}
	}

	//Run shutdown actions
	_log->logRaw(LogLevel::INFO, "Running shutdown actions...");
	for (Action *action : _shutdownActions) {
		action->execute();
	}
	_log->logRaw(LogLevel::SUCCESS, "Shutdown actions complete.");

	_log->logRaw(LogLevel::INFO, "Shutting down...");
	_backgroundThread.stop();
}

}
