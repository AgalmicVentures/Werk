
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <map>
#include <string>

#include "Log.hpp"
#include "Loggable.hpp"

namespace werk
{

class LogManager : public Loggable
{
public:
	LogManager() {
		//Create default logs
		_nullLog = new NullLog("Null");
		add(_nullLog);
	}

	//Default logs
	NullLog *nullLog() { return _nullLog; }

	//Accessors
	Log *getLog(const std::string &name) {
		auto i = _logs.find(name);
		return i == _logs.end() ? nullptr : i->second;
	}
	void add(Log *log) {
		//TODO: check if already registered
		_logs[log->name()] = log;
	}

	void logTo(Log *log) const override;

private:
	//Map of all logs and some default logs
	std::map<std::string, Log *> _logs;
	NullLog *_nullLog;
};

}
