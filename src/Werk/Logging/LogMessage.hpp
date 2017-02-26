
#pragma once

#include <cstdint>
#include <cstring>

#include "Log.hpp"

namespace werk {

/**
 * Log message container for passing between threads.
 */
struct LogMessage
{
private:
	//Some controls that no one outside this class should depend on
	static const size_t totalMessageSize = 1024;
	static const size_t headerSize = 32; //Not actually the size of the header, just has to be at least as big

public:

	static const size_t maxLineLength = totalMessageSize - headerSize;

	//Header
	uint64_t sequenceNumber;
	uint64_t time;
	LogLevel level;
	size_t length;

	//Message
	char message[maxLineLength];

	LogMessage() { }
	LogMessage(const LogMessage &m) = default;
	LogMessage &operator=(const LogMessage &m) = default;
};

}
