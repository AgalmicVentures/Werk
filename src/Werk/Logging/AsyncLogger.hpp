
#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <cstdint>
#include <cstdio>

#include "Logger.hpp"

#include "Werk/Threading/BackgroundTask.hpp"

namespace werk {

class AsyncLogger : public Logger
{
public:

	AsyncLogger(werk::Clock *clock, FILE *file=stdout) :
		Logger(clock), _file(file) { }

	virtual void log(LogLevel level, const char *format, ...) override;
	virtual void logRaw(LogLevel level, const char *rawMessage) override;

	void writeLogs();

private:
	FILE *_file;

	uint64_t _nextSendSequenceNumber = 0;
	boost::lockfree::spsc_queue<LogMessage, boost::lockfree::capacity<2048> > _messages;
	uint64_t _nextReceiveSequenceNumber = 0;
};

class BackgroundLogWriterTask : public BackgroundTask
{
public:
	BackgroundLogWriterTask(AsyncLogger *logger) : _logger(logger) { }

	virtual void execute() { _logger->writeLogs(); }

private:
	AsyncLogger *_logger;
};

}
