
#pragma once

#include <cinttypes>
#include <cstdarg>
#include <cstdio>

#include "LogMessage.hpp"

#include "Werk/OS/Time.hpp"

namespace werk {

class Logger
{
public:

	//Clock used for timestamping messages (may not be real time, e.g. in a simulation)
	Logger(werk::Clock *clock) : _clock(clock) { }
	virtual ~Logger() { }

	const werk::Clock *clock() const { return _clock; }

	virtual void logRaw(LogLevel level, const char *message) = 0;
	virtual void log(LogLevel level, const char *format, ...) = 0;

private:
	werk::Clock *_clock;
};

class SyncLogger : public Logger
{
public:

	SyncLogger(werk::Clock *clock, FILE *file=stdout) :
		Logger(clock), _file(file) { }
	virtual ~SyncLogger() { }

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
