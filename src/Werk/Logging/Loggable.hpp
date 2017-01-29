
#pragma once

#include "Werk/Utility/Action.hpp"

#include "LogMessage.hpp"

namespace werk
{

class Log;

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
