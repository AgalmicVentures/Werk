
#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <cstdint>
#include <cstdio>

#include "Log.hpp"

#include "Werk/Threading/BackgroundTask.hpp"

namespace werk {

/**
 * A log that can do its I/O on a `BackgroundThread`.
 */
class AsyncLog : public Log, public BackgroundTask
{
public:

	AsyncLog(const std::string &name, werk::Clock *clock, FILE *file=stdout, const std::string &taskName="AsyncLog") :
		Log(name, clock), BackgroundTask(taskName), _file(file) { }

	virtual void log(LogLevel level, const char *format, ...) override;
	virtual void logRaw(LogLevel level, const char *rawMessage) override;

	//Background task to write the logs
	virtual void executeTask() override;

private:
	FILE *_file;

	uint64_t _nextSendSequenceNumber = 0;
	boost::lockfree::spsc_queue<LogMessage, boost::lockfree::capacity<2048> > _messages;
	uint64_t _nextReceiveSequenceNumber = 0;
};

}
