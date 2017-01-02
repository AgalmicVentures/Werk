
#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <cstdio>
#include <thread>
#include <time.h>
#include <vector>

#include "Logger.hpp"

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
	uint64_t _nextReceiveSequenceNumber = 0;
	boost::lockfree::spsc_queue<LogMessage, boost::lockfree::capacity<2048> > _messages;
};

//TODO: eventually this will be subsumed by a general house-keeping task system
class AsycLogWriter
{
public:

	AsycLogWriter(long frequencyNs=100ul * 1000 * 1000) : _frequencyNs(frequencyNs) {
		_thread = std::thread(&AsycLogWriter::loggingThread, this);
	}
	~AsycLogWriter() { stop(); }

	void setFrequencyNs(long frequencyNs) { _frequencyNs = frequencyNs; }
	void addLogger(AsyncLogger *logger) { _loggers.push_back(logger); }

	void stop() {
		if (_running) {
			_running = false;
			_thread.join();
		}
	}

private:
	timespec _delay;
	std::vector<AsyncLogger *> _loggers;

	volatile uint64_t _frequencyNs;
	volatile bool _running = true;
	std::thread _thread;

	void loggingThread();
};

}
