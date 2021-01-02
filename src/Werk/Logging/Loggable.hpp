
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

#include "Werk/Utility/Action.hpp"

#include "Log.hpp"

namespace werk
{

/**
 * Loggable interface, for objects that can be logged.
 */
class Loggable
{
public:
	virtual void logTo(Log *log) const = 0;
};

/**
 * Implementation of loggable that outputs a constant string.
 */
class StringLoggable : public Loggable
{
public:
	StringLoggable(const std::string value, LogLevel level=LogLevel::INFO) :
		_value(value), _level(level) { }

	const std::string &value() const { return _value; }

	void logTo(Log *log) const override;

private:
	std::string _value;
	LogLevel _level;
};

/**
 * Action that logs a `Loggable` to a specific `Log`.
 */
class LogAction : public Action
{
public:
	LogAction(const std::string &name, const Loggable *loggable, Log *log) :
		Action(name), _loggable(loggable), _log(log) { }

	void execute() override {
		_loggable->logTo(_log);
	}

private:
	const Loggable *_loggable;
	Log *_log;
};

}
