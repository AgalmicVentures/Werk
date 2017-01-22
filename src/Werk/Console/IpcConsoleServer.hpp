
#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>

#include "Werk/Utility/NamedObject.hpp"

#include "ConsoleMessage.hpp"

namespace werk
{

/**
 * A console server that uses an interprocess queue to receive messages.
 */
class IpcConsoleServer : public NamedObject
{
public:

	//In some situations, a queue may be left behind
	static void remove(const std::string &name) {
		boost::interprocess::message_queue::remove(name.c_str());
	}

	IpcConsoleServer(const std::string &name, uint32_t maxMessages=1024) :
		NamedObject(name),
		_queue(boost::interprocess::create_only, name.c_str(), maxMessages, sizeof(ConsoleMessage)) { }
	~IpcConsoleServer() { remove(name()); }

	bool receive(uint32_t &sequenceNumber, std::string &message);

private:
	boost::interprocess::message_queue _queue;

	//TODO: sequence number management
};

}
