
#pragma once

#include <cinttypes>
#include <cstdarg>
#include <cstdio>
#include <string>

#include "LogMessage.hpp"

#include "Werk/OS/Time.hpp"

namespace werk {

class Log
{
public:

	//Clock used for timestamping messages (may not be real time, e.g. in a simulation)
	Log(const std::string &name, werk::Clock *clock) : _name(name), _clock(clock) { }
	virtual ~Log() { }

	const std::string &name() const { return _name; }
	const werk::Clock *clock() const { return _clock; }

	virtual void logRaw(LogLevel level, const char *message) = 0;
	virtual void log(LogLevel level, const char *format, ...) = 0;

private:
	std::string _name;
	werk::Clock *_clock;
};

class NullLog : public Log
{
public:

	NullLog(const std::string &name) : Log(name, nullptr) { }

	virtual void logRaw(LogLevel /*level*/, const char * /*message*/) override { }
	virtual void log(LogLevel /*level*/, const char * /*format*/, ...) override { }
};

class SyncLog : public Log
{
public:

	SyncLog(const std::string &name, werk::Clock *clock, FILE *file=stdout) :
		Log(name, clock), _file(file) { }
	virtual ~SyncLog() { }

	virtual void log(LogLevel level, const char *format, ...) override {
		va_list args;
		va_start(args, format);
		std::fprintf(_file, "%" PRIu64 " [%10s] ", clock()->time(), logLevels[static_cast<size_t>(level)]);
		std::vfprintf(_file, format, args);
		std::fprintf(_file, "\n");
		va_end(args);
	}
	virtual void logRaw(LogLevel level, const char *rawMessage) override {
		std::fprintf(_file, "%" PRIu64 " [%10s] %s\n", clock()->time(), logLevels[static_cast<size_t>(level)], rawMessage);
	}

private:
	FILE *_file;
};

}
