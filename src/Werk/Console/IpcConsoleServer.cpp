
#include "IpcConsoleServer.hpp"

namespace werk
{

bool IpcConsoleServer::receive(uint32_t &sequenceNumber, std::string &message)
{
	//Read the message
	char buffer[sizeof(ConsoleMessage)];
	size_t receivedSize;
	unsigned int priority;
	if (!_queue.try_receive(buffer, sizeof(ConsoleMessage), receivedSize, priority)) {
		return false;
	}

	//Deserialize the message
	ConsoleMessage *consoleMessage = (ConsoleMessage *) buffer;
	sequenceNumber = consoleMessage->sequenceNumber;
	message = consoleMessage->message;
	return true;
}

}
