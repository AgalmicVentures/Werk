
#pragma once

#include <cstdarg>
#include <cstdint>
#include <limits>
#include <string>

#include "Werk/Logging/Log.hpp"

namespace werk
{

/**
 * Abstract config class to allow integration with other configuration systems.
 */
class Config
{
public:

	Config(Log *log) : _log(log) { }

	//Basic value-as-string accessor - this is the method that every inheriting class must override (return NULL if the key is missing)
	virtual const char *getStringRaw(const std::string &key) const = 0;

	//Basic types
	virtual const char *getString(const std::string &key, const char *defaultValue=nullptr, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::INFO, "<Config> [%s] = %s [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::INFO, "<Config> [%s] = %s%s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue;
	}
	virtual double getDouble(const std::string &key, double defaultValue=0, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::INFO, "<Config> [%s] = %f [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::INFO, "<Config> [%s] = %f%s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue != nullptr ? std::stod(stringValue) : defaultValue;
	}
	virtual int64_t getInt64(const std::string &key, int64_t defaultValue=0, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIi64 " [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIi64 "%s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue != nullptr ? std::stoll(stringValue) : defaultValue;
	}
	virtual uint64_t getUint64(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIu64 " [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIu64 "%s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue != nullptr ? std::stoull(stringValue) : defaultValue;
	}

	//TODO: more complex types e.g. durations, time

protected:
	Log *_log;
};

}
