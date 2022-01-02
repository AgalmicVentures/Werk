
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Console/IpcConsoleServer.hpp"
#include "Werk/Utility/Action.hpp"

namespace werk
{

/**
 * `Action` that receives message from a console and forwards them to a
 * `CommandManager`.
 */
//TODO: logging
class ConsoleCommandReceiver : public Action
{
public:

	ConsoleCommandReceiver(const std::string &name, IpcConsoleServer &server, CommandManager &commandManager) :
		Action(name), _server(server), _commandManager(commandManager) { }
	virtual ~ConsoleCommandReceiver() { }

	void execute() override {
		//Run all of the commands in the buffer
		uint64_t clientPid = 0;
		uint32_t sequenceNumber = 0;
		std::string message;
		while (_server.receive(clientPid, sequenceNumber, message)) {
			//Skip heartbeats
			if (message.length() > 0) {
				_commandManager.execute(message);
			}
		}
	}

private:
	IpcConsoleServer &_server;
	CommandManager &_commandManager;
};

}
