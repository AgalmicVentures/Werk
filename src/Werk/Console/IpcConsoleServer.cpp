
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

	//Deserialize the message
	const ConsoleMessage *consoleMessage = (const ConsoleMessage *) buffer;
	clientPid = consoleMessage->clientPid;
	sequenceNumber = consoleMessage->sequenceNumber;
	message = consoleMessage->message;

	//Check sequence numbers numbers versus clients
	const auto i = _sequenceNumbers.find(clientPid);
	if (i == _sequenceNumbers.end()) {
		_log->log(LogLevel::SUCCESS, "<IpcConsoleServer> Connected new client %lu", clientPid);
		_sequenceNumbers[clientPid] = sequenceNumber;
	} else {
		uint64_t lastSequenceNumber = i->second;
		if (sequenceNumber < lastSequenceNumber) {
			_log->log(LogLevel::ERROR, "<IpcConsoleServer> Received out of order sequence number from client %lu: got %lu vs last %lu",
				clientPid, sequenceNumber, lastSequenceNumber);
		} else if (sequenceNumber == lastSequenceNumber) {
			_log->log(LogLevel::ERROR, "<IpcConsoleServer> Received duplicate sequence number from client %lu: got %lu vs last %lu",
				clientPid, sequenceNumber, lastSequenceNumber);
		} else if (sequenceNumber > lastSequenceNumber + 1) {
			_log->log(LogLevel::WARNING, "<IpcConsoleServer> Missing sequence numbers from client %lu: got %lu vs last %lu",
				clientPid, sequenceNumber, lastSequenceNumber);
		}
	}
	_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleServer.received\",\"clientPid\":%lu,\"commandLine\":\"%s\"}", clientPid, message.c_str());

	return true;
}

}
