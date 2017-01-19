
#pragma once

#include <map>
#include <string>

namespace werk
{

class LogManager
{
public:
	LogManager() {
		//Create default logs
		_nullLog = new NullLog("null");
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

private:
	//Map of all logs and some default logs
	std::map<std::string, Log *> _logs;
	NullLog *_nullLog;
};

}
