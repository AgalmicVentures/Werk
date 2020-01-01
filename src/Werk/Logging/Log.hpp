
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <cinttypes>
#include <cstdarg>
#include <cstdio>
#include <string>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/NamedObject.hpp"

namespace werk
{

/**
 * Level of the log, ordered to allow simple filtering.
 */
enum class LogLevel
{
	CRITICAL,
	ERROR,
	WARNING,
	ALERT,
	SUCCESS,
	CONFIG,
	INFO,
	DETAIL,
	JSON,
	TRACE
};
extern const char *logLevels[];

/**
 * Top level abstract log class which allows the logging backend to accomodate different
 * situations (e.g. real time processing of input versus high speed playback for simulation).
 */
class Log : public NamedObject
{
public:

	//Clock used for timestamping messages (may not be real time, e.g. in a simulation)
	Log(const std::string &name, const werk::Clock *clock) : NamedObject(name), _clock(clock) { }
	virtual ~Log() { }

	const werk::Clock *clock() const { return _clock; }

	virtual void logRaw(LogLevel level, const char *message) = 0;
	virtual void log(LogLevel level, const char *format, ...) = 0;

private:
	const werk::Clock *_clock;
};

/**
 * The /dev/null of logs -- drops all input.
 */
class NullLog : public Log
{
public:

	NullLog(const std::string &name) : Log(name, nullptr) { }

	virtual void logRaw(LogLevel /*level*/, const char * /*message*/) override { }
	virtual void log(LogLevel /*level*/, const char * /*format*/, ...) override { }
};

/**
 * A log that synchronously writes to a `FILE`. If you want to defer IO to another thread,
 * look at `AsyncLog`.
 */
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
