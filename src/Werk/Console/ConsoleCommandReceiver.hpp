
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

	void execute() override {
		//Run all of the commands in the buffer
		uint32_t sequenceNumber = 0;
		std::string message;
		while (_server.receive(sequenceNumber, message)) {
			_commandManager.execute(message);
		}
	}

private:
	IpcConsoleServer &_server;
	CommandManager &_commandManager;
};

}
