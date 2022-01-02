
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

#include <boost/algorithm/string/join.hpp>

#include "Werk/Logging/Log.hpp"

#include "Command.hpp"

namespace werk
{

/**
 * Command that echoes its arguments.
 */
class EchoCommand : public Command
{
public:

	EchoCommand(Log *log, LogLevel level=LogLevel::INFO) :
		Command("Echoes the arguments to the log."),
		_log(log), _level(level) { }
	virtual ~EchoCommand() { }

	bool execute(const std::vector<std::string> &arguments) override {
		//Join all but the first argument since the command name needn't be echoed
		_log->logRaw(_level, boost::algorithm::join(boost::make_iterator_range(arguments.begin() + 1, arguments.end()), " ").c_str());
		return true;
	}

private:
	Log *_log;
	LogLevel _level;
};

}
