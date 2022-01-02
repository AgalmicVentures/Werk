
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

#include "IpcConsoleClient.hpp"

#include "ConsoleMessage.hpp"

#include "Werk/OS/Time.hpp"

namespace werk
{

bool IpcConsoleClient::send(const std::string &message)
{
	ConsoleMessage consoleMessage;
	consoleMessage.magic = IPC_CONSOLE_MAGIC;
	consoleMessage.version = IPC_CONSOLE_VERSION;
	consoleMessage.clientPid = _pid;
	consoleMessage.time = epochTime();
	consoleMessage.sequenceNumber = _nextSequenceNumber;

	//Copy the message
	std::strncpy(consoleMessage.message, message.c_str(), sizeof(consoleMessage.message));
	consoleMessage.message[sizeof(consoleMessage.message) - 1] = 0;

	//Try to send it
	if (!_queue.try_send(&consoleMessage, sizeof(consoleMessage), 0)) {
		return false;
	}

	++_nextSequenceNumber;
	return true;
}

}
