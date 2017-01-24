
#pragma once

#include <map>
#include <string>

#include "Log.hpp"
#include "Loggable.hpp"

namespace werk
{

class LogManager : public Loggable
{
public:
	LogManager() {
		//Create default logs
		_nullLog = new NullLog("Null");
		add(_nullLog);
	}

	//Default logs
	NullLog *nullLog() { return _nullLog; }

	//Accessors
	Log *getLog(const std::string &name) {
		auto i = _logs.find(name);
		return i == _logs.end() ? nullptr : i->second;
	}
	void add(Log *log) {
		//TODO: check if already registered
		_logs[log->name()] = log;
	}

	void logTo(Log *log) const override;

private:
	//Map of all logs and some default logs
	std::map<std::string, Log *> _logs;
	NullLog *_nullLog;
};

}
