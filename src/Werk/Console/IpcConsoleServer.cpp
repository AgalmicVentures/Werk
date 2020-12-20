
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

#include "IpcConsoleServer.hpp"

#include <cinttypes>

#include "Werk/Logging/Log.hpp"
#include "Werk/OS/Time.hpp"

namespace werk
{

bool IpcConsoleServer::receive(uint64_t &clientPid, uint32_t &sequenceNumber, std::string &message)
{
	//Read the message
	char buffer[sizeof(ConsoleMessage)];
	size_t receivedSize;
	unsigned int priority;
	if (!_queue.try_receive(buffer, sizeof(ConsoleMessage), receivedSize, priority)) {
		return false;
	}
	const ConsoleMessage *consoleMessage = (const ConsoleMessage *) buffer;

	//Check the magic
	if (IPC_CONSOLE_MAGIC != consoleMessage->magic) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.badMagic\",\"magic\":%" PRIu32 ",\"expectedMagic\":%" PRIu32 "}",
			clientPid, consoleMessage->magic, IPC_CONSOLE_MAGIC);
		return false;
	}

	//Check the version
	if (IPC_CONSOLE_VERSION != consoleMessage->version) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.mistmatchedVersion\",\"version\":%" PRIu32 ",\"expectedVersion\":%" PRIu32 "}",
			clientPid, consoleMessage->version, IPC_CONSOLE_VERSION);
		return false;
	}

	//Magic is good and version matches, deserialize the values
	clientPid = consoleMessage->clientPid;
	sequenceNumber = consoleMessage->sequenceNumber;
	message = consoleMessage->message;

	//Check sequence numbers numbers versus clients
	auto i = _clientStates.find(clientPid);
	if (i == _clientStates.end()) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.clientConnected\",\"clientPid\":%" PRIu64 "}", clientPid);
		auto i2 = _clientStates.emplace(std::make_pair(clientPid, IpcConsoleClientState()));
		i = i2.first; //The insert can't fail
	} else {
		uint64_t lastSequenceNumber = i->second.lastSequenceNumber;
		if (sequenceNumber < lastSequenceNumber) {
			_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.outOfOrderSequence\",\"clientPid\":%" PRIu64 ",\"sequenceNumber\":%" PRIu64 ",\"lastSequenceNumber\":%" PRIu64 "}",
				clientPid, sequenceNumber, lastSequenceNumber);
		} else if (sequenceNumber == lastSequenceNumber) {
			_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.duplicateSequence\",\"clientPid\":%" PRIu64 ",\"sequenceNumber\":%" PRIu64 ",\"lastSequenceNumber\":%" PRIu64 "}",
				clientPid, sequenceNumber, lastSequenceNumber);
		} else if (sequenceNumber > lastSequenceNumber + 1) {
			_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.missingSequence\",\"clientPid\":%" PRIu64 ",\"sequenceNumber\":%" PRIu64 ",\"lastSequenceNumber\":%" PRIu64 "}",
				clientPid, sequenceNumber, lastSequenceNumber);
		}

		//Always try to resync
		i->second.lastSequenceNumber = sequenceNumber;
	}

	//Check the timestamps as well, to ensure timely processing -- anything more than 3s old is stale and should warn the user
	const uint64_t time = _realTimeClock->time();
	if (time > consoleMessage->time + 3000l * 1000 * 1000) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.stale\",\"clientPid\":%" PRIu64 ",\"sequenceNumber\":%" PRIu64 ",\"timestamp\":%" PRIu64 ",\"delta\":%" PRIu64 "}",
			clientPid, sequenceNumber, consoleMessage->time, time - consoleMessage->time);
	}

	//Update timer state
	//Everything counts as a heartbeat, but commands must have content
	_lastHeartbeatTime = time;
	i->second.lastHeartbeatTime = time;
	if (0 != message[0]) {
		_lastCommandTime = time;
		i->second.lastCommandTime = time;
	}

	//Log
	_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.received\",\"clientPid\":%" PRIu64 ",\"sequenceNumber\":%" PRIu64 ",\"commandLine\":\"%s\"}",
		clientPid, sequenceNumber, message.c_str());

	return true;
}

}
