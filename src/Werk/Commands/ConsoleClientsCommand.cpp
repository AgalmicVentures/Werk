
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

#include "ConsoleClientsCommand.hpp"

#include "Werk/Console/IpcConsoleServer.hpp"
#include "Werk/Logging/Log.hpp"

namespace werk
{

bool ConsoleClientsCommand::execute(const std::vector<std::string> &)
{
	//Log all console clients
	const uint64_t time = _realTimeClock->time();
	_log->log(LogLevel::INFO, "******************** Console Clients (%s) ********************",
		_ipcConsoleServer.isClientConnected() ? "Connected" : "Not Connected");
	_log->log(LogLevel::INFO, " %10s %12s %16s %16s",
		"PID", "Last Seq. #", "Last Command", "Last Heartbeat");
	_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleClient.state\",\"clientPid\":\"All\",\"lastSequenceNumber\":\"\",\"lastCommandTime\":%" PRIu64 ",\"lastHeartbeatTime\":%" PRIu64 ",\"lastCommandAgeSec\":%.1lf,\"lastHeartbeatAgeSec\":%.1lf}",
		_ipcConsoleServer.lastCommandTime(), _ipcConsoleServer.lastHeartbeatTime(),
		(time - _ipcConsoleServer.lastCommandTime()) / 1e9,
		(time - _ipcConsoleServer.lastHeartbeatTime()) / 1e9
	);
	for (auto i = _ipcConsoleServer.clientStates().begin(); i != _ipcConsoleServer.clientStates().end(); ++i) {
		_log->log(LogLevel::JSON, "{\"type\":\"ipcConsoleClient.state\",\"clientPid\":%" PRIu64 ",\"lastSequenceNumber\":%" PRIu64 ",\"lastCommandTime\":%" PRIu64 ",\"lastHeartbeatTime\":%" PRIu64 ",\"lastCommandAgeSec\":%.1lf,\"lastHeartbeatAgeSec\":%.1lf}",
			i->first, i->second.lastSequenceNumber, i->second.lastCommandTime, i->second.lastHeartbeatTime,
			(time - i->second.lastCommandTime) / 1e9,
			(time - i->second.lastHeartbeatTime) / 1e9
		);
	}
	return true;
}

}
