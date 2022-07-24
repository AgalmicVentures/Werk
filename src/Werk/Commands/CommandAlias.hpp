
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

#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>

#include "Command.hpp"

namespace werk
{

class CommandManager;

/**
 * Command that does nothing. Useful as a placeholder or for testing.
 */
class CommandAlias : public Command
{
public:
	CommandAlias(CommandManager &commandManager, const std::string &command) :
		Command("Alias for " + command), _commandManager(commandManager)
	{
		std::string trimmedCommand = boost::trim_copy(command);
		boost::split(_command, trimmedCommand, boost::is_any_of(" \t"), boost::token_compress_on);
	}
	virtual ~CommandAlias() { }

	virtual bool execute(const std::vector<std::string> &arguments) override;

protected:
	CommandManager &_commandManager;
	std::vector<std::string> _command;
};

}
