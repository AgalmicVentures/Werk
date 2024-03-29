
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <boost/interprocess/ipc/message_queue.hpp>
#include <map>

#include "Werk/Utility/Attributes.hpp"
#include "Werk/Utility/NamedObject.hpp"

#include "ConsoleMessage.hpp"

namespace werk
{

class Clock;
class Log;

struct IpcConsoleClientState
{
	uint64_t lastSequenceNumber = 0;
	uint64_t lastHeartbeatTime = 0;
	uint64_t lastCommandTime = 0;
};

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

	IpcConsoleServer(const std::string &name, Log *log, Clock *realTimeClock, uint32_t maxMessages=1024) :
		NamedObject(name),
		_log(log),
		_realTimeClock(realTimeClock),
		_queue(boost::interprocess::create_only, name.c_str(), maxMessages, sizeof(ConsoleMessage)) { }
	~IpcConsoleServer() { remove(name()); }

	CHECKED bool isClientConnected() const { return _clientConnected; }
	CHECKED uint64_t lastCommandTime() const { return _lastCommandTime; }
	CHECKED uint64_t lastHeartbeatTime() const { return _lastHeartbeatTime; }
	CHECKED const std::map<uint64_t, IpcConsoleClientState> &clientStates() const { return _clientStates; }

	CHECKED bool receive(uint64_t &clientPid, uint32_t &sequenceNumber, std::string &message);
	void update();

private:
	Log *_log;
	Clock *_realTimeClock;
	boost::interprocess::message_queue _queue;

	//Client state, including some helpful globals
	bool _clientConnected = false;
	uint64_t _lastCommandTime = 0;
	uint64_t _lastHeartbeatTime = 0;
	std::map<uint64_t, IpcConsoleClientState> _clientStates;
};

}
