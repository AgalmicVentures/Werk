
#pragma once

#include <cstring>

namespace werk {

//Level of the log, ordered to allow simple filtering
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

//Used for passing log messages between threads
struct LogMessage
{
private:
	//Some controls that no one outside this class should depend on
	static const size_t totalMessageSize = 1024;
	static const size_t headerSize = 32; //Not actually the size of the header, just has to be at least as big
	static const size_t maxLineLength = totalMessageSize - headerSize;

public:
	//Header
	uint64_t sequenceNumber;
	uint64_t time;
	LogLevel level;
	size_t length;

	//Message
	char message[maxLineLength];

	LogMessage() { }
	LogMessage(const LogMessage &m) {
		std::memcpy(this, &m, headerSize + m.length);
	}
	LogMessage &operator=(const LogMessage &m) {
		std::memcpy(this, &m, headerSize + m.length);
		return *this;
	}
};

}
