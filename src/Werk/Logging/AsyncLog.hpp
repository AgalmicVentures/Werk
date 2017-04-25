
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <cstdint>
#include <cstdio>

#include "Log.hpp"
#include "LogMessage.hpp"

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
	boost::lockfree::queue<LogMessage, boost::lockfree::capacity<2048> > _messages;
	uint64_t _nextReceiveSequenceNumber = 0;
};

}
