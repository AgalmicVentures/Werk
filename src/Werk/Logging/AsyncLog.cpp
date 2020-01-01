
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

#include "AsyncLog.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>

#include "Werk/OS/Time.hpp"

namespace werk
{

//TODO: move formatting to housekeeping thread
void AsyncLog::log(LogLevel level, const char *format, ...)
{
	//Create the buffer
	LogMessage message;
	message.sequenceNumber = _nextSendSequenceNumber++;
	message.time = clock()->time();
	message.level = level;

	//Format the message
	va_list args;
	va_start(args, format);
	const int n = std::vsnprintf(message.message, LogMessage::maxLineLength, format, args);
	if (n < 0) {
		//TODO: error handling
		message.length = LogMessage::maxLineLength;
	} else if (n >= static_cast<int>(LogMessage::maxLineLength)) {
		//Warn about truncation
		std::fprintf(stderr, "Truncated log message: %s\n", message.message);
		message.length = LogMessage::maxLineLength;
	} else {
		message.length = static_cast<size_t>(n) + 1;
	}
	va_end(args);

	_messages.push(message);
}

void AsyncLog::logRaw(LogLevel level, const char *rawMessage)
{
	LogMessage message;
	message.sequenceNumber = _nextSendSequenceNumber++;
	message.time = clock()->time();
	message.level = level;

	//Log the message
	message.length = LogMessage::maxLineLength; //TODO: this could be calculated on the fly
	std::strncpy(message.message, rawMessage, message.length);
	message.message[message.length - 1] = 0;

	_messages.push(message);
}

void AsyncLog::execute()
{
	bool wrote = false;

	LogMessage message;
	while (_messages.pop(message)) {
		if (message.sequenceNumber != _nextReceiveSequenceNumber) {
			std::fprintf(_file, "{\"t\":%" PRIu64 ",\"n\":%" PRIu64 ",\"level\":\"%s\",\"message\":\"Incorrect log sequence number; expecting %" PRIu64 " but received %" PRIu64 "\"}\n",
				message.time,
				_nextReceiveSequenceNumber,
				logLevels[static_cast<size_t>(LogLevel::CRITICAL)],
				_nextReceiveSequenceNumber,
				message.sequenceNumber);
		}
		_nextReceiveSequenceNumber = message.sequenceNumber + 1;

		if (message.level == LogLevel::JSON) {
			std::fprintf(_file, "{\"t\":%" PRIu64 ",\"n\":%" PRIu64 ",\"data\":%s}\n",
				message.time, message.sequenceNumber, message.message);
		} else {
			std::fprintf(_file, "{\"t\":%" PRIu64 ",\"n\":%" PRIu64 ",\"level\":\"%s\",\"message\":\"%s\"}\n",
				message.time, message.sequenceNumber, logLevels[static_cast<size_t>(message.level)], message.message);
		}

		wrote = true;
	}

	//Always flush, it's a log
	if (wrote) {
		std::fflush(_file);
	}
}

}
