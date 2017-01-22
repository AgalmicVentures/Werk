
#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>

#include "Werk/Utility/NamedObject.hpp"

namespace werk
{

/**
 * A console client that use interprocess queues to send messages.
 */
class IpcConsoleClient : public NamedObject
{
public:
	IpcConsoleClient(const std::string &name) :
		NamedObject(name),
		_queue(boost::interprocess::open_only, name.c_str()) { }

	bool send(const std::string &message);

private:
	boost::interprocess::message_queue _queue;

	uint32_t _nextSequenceNumber = 0;
};

}
