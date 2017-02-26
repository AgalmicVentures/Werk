
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
	int n = std::vsnprintf(message.message, LogMessage::maxLineLength, format, args);
	if (n < 0) {
		//TODO: error handling
		message.length = LogMessage::maxLineLength;
	} else if (n >= static_cast<int>(LogMessage::maxLineLength)) {
		//Warn about truncation
		std::fprintf(stderr, "Truncated log message: %s\n", message.message);
		message.length = LogMessage::maxLineLength;
	} else {
		message.length = n + 1;
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
