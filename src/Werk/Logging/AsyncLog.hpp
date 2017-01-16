
#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <cstdint>
#include <cstdio>

#include "Log.hpp"

#include "Werk/Utility/Action.hpp"

namespace werk {

/**
 * A log that can do its I/O on a `BackgroundThread`.
 */
class AsyncLog : public Log, public Action
{
public:

	AsyncLog(const std::string &name, const werk::Clock *clock, FILE *file=stdout) :
		Log(name, clock), Action(name + "_Writer"), _file(file) { }

	virtual void log(LogLevel level, const char *format, ...) override;
	virtual void logRaw(LogLevel level, const char *rawMessage) override;

	//Background task to write the logs
	virtual void execute() override;

private:
	FILE *_file;

	uint64_t _nextSendSequenceNumber = 0;
	boost::lockfree::spsc_queue<LogMessage, boost::lockfree::capacity<2048> > _messages;
	uint64_t _nextReceiveSequenceNumber = 0;
};

}
