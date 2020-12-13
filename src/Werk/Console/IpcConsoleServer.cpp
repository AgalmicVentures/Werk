
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
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.badMagic\",\"magic\":%u,\"expectedMagic\":%u}",
			clientPid, consoleMessage->magic, IPC_CONSOLE_MAGIC);
		return false;
	}

	//Check the version
	if (IPC_CONSOLE_VERSION != consoleMessage->version) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.mistmatchedVersion\",\"version\":%u,\"expectedVersion\":%u}",
			clientPid, consoleMessage->version, IPC_CONSOLE_VERSION);
		return false;
	}

	//Magic is good and version matches, deserialize the values
	clientPid = consoleMessage->clientPid;
	sequenceNumber = consoleMessage->sequenceNumber;
	message = consoleMessage->message;

	//Check sequence numbers numbers versus clients
	const auto i = _sequenceNumbers.find(clientPid);
	if (i == _sequenceNumbers.end()) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.clientConnected\",\"clientPid\":%lu}", clientPid);
		_sequenceNumbers[clientPid] = sequenceNumber;
	} else {
		uint64_t lastSequenceNumber = i->second;
		if (sequenceNumber < lastSequenceNumber) {
			_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.outOfOrderSequence\",\"clientPid\":%lu,\"sequenceNumber\":%lu,\"lastSequenceNumber\":%lu}",
				clientPid, sequenceNumber, lastSequenceNumber);
		} else if (sequenceNumber == lastSequenceNumber) {
			_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.duplicateSequence\",\"clientPid\":%lu,\"sequenceNumber\":%lu,\"lastSequenceNumber\":%lu}",
				clientPid, sequenceNumber, lastSequenceNumber);
		} else if (sequenceNumber > lastSequenceNumber + 1) {
			_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.missingSequence\",\"clientPid\":%lu,\"sequenceNumber\":%lu,\"lastSequenceNumber\":%lu}",
				clientPid, sequenceNumber, lastSequenceNumber);
		}

		//Always try to resync
		i->second = sequenceNumber;
	}

	//Check the timestamps as well, to ensure timely processing -- anything more than 3s old is stale and should warn the user
	const uint64_t time = _realTimeClock->time();
	if (time > consoleMessage->time + 3000l * 1000 * 1000) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.stale\",\"clientPid\":%lu,\"sequenceNumber\":%lu,\"timestamp\":%lu,\"delta\":%lu}",
			clientPid, sequenceNumber, consoleMessage->time, time - consoleMessage->time);
	}

	//Log
	_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.received\",\"clientPid\":%lu,\"commandLine\":\"%s\"}", clientPid, message.c_str());

	return true;
}

}
